#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Utils.h>

enum MasterSlaveState
{
    DISABLED,
    MASTER,
    SLAVE
};

class I2cMaster
{
private:
    static TwoWire &wire;
    static MasterSlaveState &state;

public:
    void begin() const;

    uint8_t write(const uint8_t slaveAddress, const uint8_t *bytes, uint8_t size) const;
    template <uint8_t size>
    uint8_t write(const uint8_t slaveAddress, const uint8_t (&data)[size]) const;
    template <typename T>
    uint8_t write(const uint8_t slaveAddress, const T value) const;
    uint8_t read(const uint8_t slaveAddress, uint8_t *bytes, uint8_t size);
    template <uint8_t size>
    uint8_t read(const uint8_t slaveAddress, uint8_t (&bytes)[size]);
    template <typename T>
    uint8_t read(const uint8_t slaveAddress, T &value);
};

class I2cSlave
{
    class Reader;
    class Writer;

private:
    static TwoWire &wire; // it has to be static to be accessible during interrupts.
    static MasterSlaveState &state;
    static void (*onMasterRequest)(Writer &, void *);
    static void *onMasterRequestObject;
    static void (*onMasterReceive)(Reader &, void *);
    static void *onMasterReceiveObject;

    static void onRequest(void (*callback)());
    static void onReceive(void (*callback)(int));

public:
    void begin(uint8_t slaveAddress);
    void onRequest(void (*onMasterRequest)(Writer &));
    template <typename T>
    void onRequest(void (*onMasterRequest)(Writer &, T &), T &obj);
    void onReceive(void (*onMasterReceive)(Reader &));
    template <typename T>
    void onReceive(void (*onMasterReceive)(Reader &, T &), T &obj);

    class Reader
    {
    private:
        uint8_t &buffer[BUFFER_LENGTH];
        uint8_t size;

    public:
        Reader(uint8_t &buffer[BUFFER_LENGTH], uint8_t size) : buffer(buffer), size(size) {}

        uint8_t read(uint8_t *bytes, uint8_t &size);
        template <uint8_t size>
        uint8_t read(uint8_t (&bytes)[size]);
        template <typename T>
        uint8_t read(T &value);
        uint8_t getSize();
    };

    class Writer : public Reader
    {
    public:
        uint8_t write(const uint8_t *bytes, uint8_t size);
        template <uint8_t size>
        uint8_t write(const uint8_t (&data)[size]);
        template <typename T>
        uint8_t write(const T value);
    };
};

class I2c : I2cMaster, I2cSlave
{
private:
    static TwoWire &wire;
    static MasterSlaveState state = DISABLED;

public:
    I2c(const I2c &) = delete;            // delete the copy constructor.
    void operator=(const I2c &) = delete; // delete the copy assignment operator.

    I2cMaster &master() { return *this; }
    I2cSlave &slave() { return *this; }
    void disable();
};
