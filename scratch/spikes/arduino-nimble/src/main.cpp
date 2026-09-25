#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

NimBLEServer* pServer = nullptr;
NimBLEHIDDevice* pHid = nullptr;
NimBLECharacteristic* pInputMouse = nullptr;

void setup() {
    Serial.begin(115200);
    Serial.println("Spike: Arduino + NimBLE BLE HID Mouse");

    NimBLEDevice::init("Zephyr-Spike-Arduino");
    pServer = NimBLEDevice::createServer();
    pHid = new NimBLEHIDDevice(pServer);

    pInputMouse = pHid->inputReport(1);

    pHid->manufacturer()->setValue("Zephyr");
    pHid->pnp(0x02, 0x045e, 0x0040, 0x0110);
    pHid->hidInfo(0x00, 0x01);

    const uint8_t reportMap[] = {
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x02,                    // USAGE (Mouse)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x09, 0x01,                    //   USAGE (Pointer)
        0xa1, 0x00,                    //   COLLECTION (Physical)
        0x85, 0x01,                    //     REPORT_ID (1)
        0x05, 0x09,                    //     USAGE_PAGE (Button)
        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
        0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x95, 0x03,                    //     REPORT_COUNT (3)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x75, 0x05,                    //     REPORT_SIZE (5)
        0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //     USAGE (X)
        0x09, 0x31,                    //     USAGE (Y)
        0x09, 0x38,                    //     USAGE (Wheel)
        0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
        0x75, 0x08,                    //     REPORT_SIZE (8)
        0x95, 0x03,                    //     REPORT_COUNT (3)
        0x81, 0x06,                    //     INPUT (Data,Var,Rel)
        0xc0,                          //   END_COLLECTION
        0xc0                           // END_COLLECTION
    };

    pHid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
    pHid->startServices();

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(pHid->hidService()->getUUID());
    pAdvertising->start();

    Serial.println("Advertising started");
}

void loop() {
    delay(1000);
}
