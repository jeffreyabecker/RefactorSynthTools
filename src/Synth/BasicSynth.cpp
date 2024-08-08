#include "config.h"
#include "../lib/Synthesis.h"
#include "../lib/Midi.h"

Synthesis::StaticStereoSampleBuffer<SAMPLE_BUFFER_SIZE> buffer;

static std::array<ChannelSettings, Midi::Constants::MaxChannels> channelConfigs;