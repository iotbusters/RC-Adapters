#pragma once

#include <Arduino.h>
// #include <Delegates/Proc.h>
#include <Utils.h>
#include <Wire.h>

enum MasterSlaveState { DISABLED, MASTER, SLAVE };

class I2cClient {
private:
    TwoWire *wire;
    MasterSlaveState state = DISABLED;

    bool ensureState(const MasterSlaveState comparand) const;
    TwoWire *getWire() const { return this->wire ? this->wire : &Wire; }

public:
    I2cClient(TwoWire &wire) : wire(&wire) {}

    // I2cClient(const I2cClient&) = delete;
    // void operator=(const I2cClient &) = delete;

    void masterBegin();
    uint8_t masterWrite(const uint8_t slaveAddress, const uint8_t *bytes, const uint8_t size) const;
    uint8_t masterRead(const uint8_t slaveAddress, uint8_t *bytes, const uint8_t size);

    void slaveBegin(const uint8_t slaveAddress, void (*onReceive)(int), void (*onRequest)());
    uint8_t slaveWrite(const uint8_t *bytes, const uint8_t size);
    uint8_t slaveRead(uint8_t *bytes, const uint8_t size);
    uint8_t slaveRead();

    void disable();
};

class I2cMaster {
private:
    static I2cClient *client;

public:
    I2cMaster(I2cClient &client) { I2cMaster::client = &client; }

    void begin() const;

    uint8_t write(const uint8_t slaveAddress, const uint8_t *bytes, const uint8_t size) const;
    template <uint8_t size>
    uint8_t write(const uint8_t slaveAddress, const uint8_t (&bytes)[size]) const {
        return this->write(slaveAddress, bytes, size);
    }
    template <typename T>
    uint8_t write(const uint8_t slaveAddress, const T &value) const {
        const uint8_t size = sizeof(T);
        const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        // Little-endian system
        uint8_t array[size];
        for (auto i = 0; i < size; ++i)
            array[size - 1 - i] = bytes[i];
        return this->write(slaveAddress, array);
#else
        // Big-endian system
        return this->write(slaveAddress, bytes, size);
#endif
    }

    uint8_t read(const uint8_t slaveAddress, uint8_t *bytes, const uint8_t size);
    template <uint8_t size>
    uint8_t read(const uint8_t slaveAddress, uint8_t (&bytes)[size]) {
        return this->read(slaveAddress, bytes);
    }
    template <typename T>
    uint8_t read(const uint8_t slaveAddress, T &value) {
        const uint8_t size = sizeof(T);
        const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        // Little-endian system
        uint8_t array[size];
        for (auto i = 0; i < size; ++i)
            array[size - 1 - i] = bytes[i];
        return this->read(slaveAddress, array);
#else
        // Big-endian system
        return this->read(slaveAddress, bytes, size);
#endif
    }
};

class I2cSlave {
public:
    class Reader;
    class Writer;

private:
    static I2cClient *client;

    static void (*onMasterRequestAction)(Writer *);
    static void (*onMasterReceiveAction)(Reader *);

    static void onRequest();
    static void onReceive(int bytesNumber);

public:
    I2cSlave(I2cClient &client) { I2cSlave::client = &client; }

    void begin(uint8_t slaveAddress);
    void onMasterRequest(void (*onMasterRequest)(Writer *));
    template <typename T>
    void onMasterRequest(void (*onMasterRequest)(T, Writer *), T obj);
    void onMasterReceive(void (*onMasterReceive)(Reader *));
    template <typename T>
    void onMasterReceive(void (*onMasterReceive)(T, Reader *), T obj);

    class Reader {
    private:
        uint8_t *buffer;
        uint8_t size;
        uint8_t cursor;

    public:
        Reader(uint8_t *buffer, const uint8_t size) : buffer(buffer), size(size) {}

        uint8_t read(uint8_t *bytes, const uint8_t size);
        template <typename T>
        uint8_t read(T &value);
        uint8_t available() { return size - cursor; }
    };

    class Writer {
    private:
        I2cClient *client;

    public:
        Writer(I2cClient *client) : client(client) {}

        uint8_t write(const uint8_t *bytes, const uint8_t size);
        template <uint8_t size>
        uint8_t write(const uint8_t (&data)[size]);
        template <typename T>
        uint8_t write(const T value);
    };
};

class I2c {
private:
    TwoWire &wire;

    inline I2cClient &getClient() const {
        static I2cClient instance = I2cClient(this->wire);
        return instance;
    }

public:
    I2c(TwoWire &wire) : wire(wire) {}

    I2cMaster master() const { return I2cMaster(this->getClient()); }
    I2cSlave slave() const { return I2cSlave(this->getClient()); }
    void disable() const { this->getClient().disable(); }
};
