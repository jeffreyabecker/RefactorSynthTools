#pragma once
#include <cstddef>
namespace Synthesis
{

    class SampleBuffer
    {

    public:
        virtual size_t length() = 0;
        virtual float &operator[](size_t index);
        virtual void clear()
        {
            auto bufferLength = length();
            for (size_t i = 0; i < bufferLength; i++)
            {
                (this->operator[](i)) = 0.0f;
            }
        }
        virtual void copyTo(SampleBuffer &that)
        {

            auto bufferLength = length() < that.length() ? length() : that.length();

            for (size_t i = 0; i < bufferLength; i++)
            {
                that[i] = (this->operator[])(i);
            }
        }
    };

    template <size_t BufferLength>
    class StaticSampleBuffer : public SampleBuffer
    {
    protected:
        float[BufferLength] _samples;

    public:
        StaticSampleBuffer() {}
        StaticSampleBuffer(const SampleBuffer &that)
        {
            auto thatLength = that.length();
            for (size_t i = 0; i < thatLength && i < BufferLength; i++)
            {
                _samples[i] = that[i];
            }
        }

        virtual size_t length() override { return BufferLength; };
        virtual float &operator[](size_t index) override
        {
            return _samples[index];
        }
    };

    template <size_t BufferLength>
    class ReadOnlySampleBuffer : public SampleBuffer
    {
    private:
        float (&_samples)[BufferLength];
        float _bogusSampleForReturnFromOperator;

    public:
        ReadOnlySampleBuffer(float (&samples)[BufferLength]) : _samples(samples) {}
        ReadOnlySampleBuffer(const ReadOnlySampleBuffer &that) : _samples(that._samples) {}
        virtual size_t length() override { return BufferLength; };
        virtual float &operator[](size_t index) override
        {
            _bogusSampleForReturnFromOperator = _samples[index];
            return _bogusSampleForReturnFromOperator;
        }
        virtual void clear() override
        {
            // this method intentionally left blank;
        }
    };

    template <size_t BufferLength>
    class FixedValueSampleBuffer : public SampleBuffer
    {
    private:
        float _value;
        float _bogusSampleForReturnFromOperator;

    public:
        FixedValueSampleBuffer(float value) : _value(value) {}
        FixedValueSampleBuffer(const FixedValueSampleBuffer &that) : _value(that._value) {}
        virtual size_t length() override { return BufferLength; };
        virtual float &operator[](size_t index) override
        {
            _bogusSampleForReturnFromOperator = _value;
            return _bogusSampleForReturnFromOperator;
        }
        virtual void clear() override
        {
            // this method intentionally left blank;
        }
    };
}