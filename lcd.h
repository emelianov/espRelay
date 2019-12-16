#pragma once
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define LCD_CFG "/lcd.json"
#ifndef LCD_ID
// #define LCD_ID    0x27
 #define LCD_ID    0x3F
#endif
#ifndef LCD_HEIGHT
 #define LCD_HEIGHT 2
#endif
#ifndef LCD_WIDTH
 #define LCD_WIDTH  16
#endif

LiquidCrystal_I2C * lcd;

uint32_t lcdInit() {
    uint8_t address;
    uint8_t width = LCD_WIDTH;
    uint8_t height = LCD_HEIGHT;
    cJSON* json = nullptr;
    cJSON* arr = nullptr;
    cJSON* entry = nullptr;
    cJSON* item = nullptr;

    File configFile = SPIFFS.open(CFG_LCD, "r");
    if (!configFile)
        goto cleunup;
    char* data = (char*)malloc(configFile.size() + 1);
    if (!data)
        goto cleunup;
    if (configFile.read((uint8_t*)data, configFile.size()) != configFile.size())
        goto cleunup;
    data[configFile.size()] = '/0';
    TDEBUG("lcd: config %d bytes read\n", configFile.size());
    json = cJSON_Parse(data);
    if (!json)
        goto cleunup;
    TDEBUG("lcd: JSON parsed\n");
    if (!cJSON_HasObjectItem(json, "address"))
        goto cleanup;
    item = cJSON_GetObjectItemCaseSensitive(json, "address");
    if (item && cJSON_IsNumber(item))
        address = item->valueint;
        }
    if (cJSON_HasObjectItem(json, "width")) {
        item = cJSON_GetObjectItemCaseSensitive(json, "width");
        if (item && cJSON_IsNumber(item))
          width = item->valueint;
    }
    if (cJSON_HasObjectItem(json, "height")) {
        item = cJSON_GetObjectItemCaseSensitive(json, "height");
        if (item && cJSON_IsNumber(item))
          height = item->valueint;
    }

    lcd = new LiquidCrystal_I2C(address, width, height);
    lcd->begin();

    cleanup:
    if (json) cJSON_Delete(json);
    if (data) free(data);
    if (configFile) configFile.close();
    return RUN_DELETE;
}