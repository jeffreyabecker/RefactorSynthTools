#include "BluetoothMidiSink.h"
#include <string>

Midi::BluetoothMidiSink::BluetoothMidiSink(const std::string deviceName) : _deviceName(deviceName), connected(false) {}

void Midi::BluetoothMidiSink::stop()
{
    BLEDevice::deinit();
}

bool Midi::BluetoothMidiSink::isConnected()
{
    return connected;
}

void Midi::BluetoothMidiSink::start()
{
    BLEDevice::init(_deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);
    BLEService *pService = pServer->createService(BLEUUID(MIDI_SERVICE_UUID));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(MIDI_CHARACTERISTIC_UUID),
        NIMBLE_PROPERTY::READ |
            NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::NOTIFY |
            NIMBLE_PROPERTY::WRITE_NR);
    pCharacteristic->setCallbacks(this);
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
}

void Midi::BluetoothMidiSink::setOnConnectCallback(void (*const onConnectCallback)())
{
    this->onConnectCallback = onConnectCallback;
}

void Midi::BluetoothMidiSink::setOnDisconnectCallback(void (*const onDisconnectCallback)())
{
    this->onDisconnectCallback = onDisconnectCallback;
}

void Midi::BluetoothMidiSink::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if (!connected)
        return;
    pCharacteristic->setValue(packet, packetSize);
    pCharacteristic->notify();
}

void Midi::BluetoothMidiSink::onConnect(BLEServer *pServer)
{
    connected = true;
    if (onConnectCallback != nullptr)
        onConnectCallback();
}

void Midi::BluetoothMidiSink::onDisconnect(BLEServer *pServer)
{
    connected = false;
    if (onDisconnectCallback != nullptr)
        onDisconnectCallback();
    pServer->startAdvertising();
}

void Midi::BluetoothMidiSink::onWrite(BLECharacteristic *pCharacteristic)
{
    vTaskDelay(0); // We leave some time for the IDLE task call esp_task_wdt_reset_watchdog
                   // See comment from atanisoft here : https://github.com/espressif/arduino-esp32/issues/2493
}

void Midi::BluetoothMidiSink::send(Midi::MidiMessage message)
{
    if (!connected)
        return;
    uint8_t packet[5];
    auto length = message.length();
    message.getData((uint8_t *)&packet, 2, length);

    auto t = millis();
    uint8_t headerByte = (1 << 7) | ((t >> 7) & ((1 << 6) - 1));
    uint8_t timestampByte = (1 << 7) | (t & ((1 << 7) - 1));

    packet[0] = headerByte;
    packet[1] = timestampByte;

    pCharacteristic->setValue(packet, length + 2);
    pCharacteristic->notify();
}