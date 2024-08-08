#pragma once
#include <cstddef>
namespace Synthesis
{

    template <size_t BufferLength = 48>
    class SampleBuffer
    {

    public:
        virtual size_t length() { return BufferLength; }
        virtual float &operator[](size_t index);
        virtual void clear()
        {
            for (size_t i = 0; i < BufferLength; i++)
            {
                (this->operator[](i)) = 0.0f;
            }
        }
        virtual void copyTo(SampleBuffer &that)
        {
            for (size_t i = 0; i < BufferLength; i++)
            {
                that[i] = (this->operator[])(i);
            }
        }
    };
    template <size_t BufferLength = 48>
    class StereoSampleBuffer
    {
    protected:
        SampleBuffer &_left;
        SampleBuffer &_right;

    public:
        StereoSampleBuffer(SampleBuffer &l, SampleBuffer &r) : _left(l), _right(r)
        {
        }
        size_t length() { return BufferLength; }
        void clear()
        {
            _left.clear();
            _right.clear();
        }
        void copyTo(StereoSampleBuffer &that)
        {
            _left.copyTo(that.left);
            _right.copyTo(that.right);
        }
        SampleBuffer &left() { return _left; }
        SampleBuffer &right() { return _right; }
    };

    template <size_t BufferLength = 48>
    class StaticSampleBuffer : public SampleBuffer<BufferLength>
    {
    protected:
        float[BufferLength] _samples;

    public:
        StaticSampleBuffer() {}

        virtual float &operator[](size_t index) override
        {
            return _samples[index];
        }
    };
    template <size_t BufferLength = 48>
    class StaticStereoSampleBuffer : public StereoSampleBuffer
    {
    private:
        StaticSampleBuffer staticLeft;
        StaticSampleBuffer staticRight;

    public:
        StaticStereoSampleBuffer() : _left(staticLeft), _right(staticRight) {}
    };

    template <size_t BufferLength = 48>
    class ReadOnlySampleBuffer : public SampleBuffer<BufferLength>
    {
    private:
        float (&_samples)[BufferLength];
        float _bogusSampleForReturnFromOperator;

    public:
        ReadOnlySampleBuffer(float (&samples)[BufferLength]) : _samples(samples) {}
        ReadOnlySampleBuffer(const ReadOnlySampleBuffer &that) : _samples(that._samples) {}

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

    template <size_t BufferLength = 48>
    class FixedValueSampleBuffer : public SampleBuffer<BufferLength>
    {
    private:
        float _value;
        float _bogusSampleForReturnFromOperator;

    public:
        FixedValueSampleBuffer(float value) : _value(value) {}
        FixedValueSampleBuffer(const FixedValueSampleBuffer &that) : _value(that._value) {}

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