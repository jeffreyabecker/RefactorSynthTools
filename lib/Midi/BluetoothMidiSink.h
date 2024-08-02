#pragma once

#include <NimBLEDevice.h>
#include "MidiSink.h"
#include "MidiMessages.h"

namespace Midi
{
    class BluetoothMidiSink : public MidiSink, public BLEServerCallbacks, public BLECharacteristicCallbacks
    {
    public:
        BluetoothMidiSink(const std::string deviceName);
        void start() override;
        void stop() override;
        bool isConnected() override;
        void send(const Midi::MidiMessage message) override;
        void setOnConnectCallback(void (*const onConnectCallback)());
        void setOnDisconnectCallback(void (*const onDisconnectCallback)());

    private:
        std::string _deviceName;
        bool connected = false;
        const std::string MIDI_SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
        const std::string MIDI_CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";
        virtual void sendPacket(uint8_t *packet, uint8_t packetSize);
        void onConnect(BLEServer *pServer);
        void onDisconnect(BLEServer *pServer);

        void (*onConnectCallback)() = nullptr;
        void (*onDisconnectCallback)() = nullptr;
        BLECharacteristic *pCharacteristic = nullptr;
        void onWrite(BLECharacteristic *pCharacteristic);
    };


}
