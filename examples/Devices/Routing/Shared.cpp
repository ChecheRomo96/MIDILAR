#include "Shared.h"
#include <MIDILAR_Devices.h>

namespace MIDILARExamples::Devices::Routing {
using namespace MIDILAR::Protocol;
using namespace MIDILAR::Devices;
namespace {
struct Sink {
    PrintFunction print;
    const char* label;
    void Process(const Packet& packet) {
        print(label);
        PrintPacket(print, packet);
        PrintNumber(print, "Channel", packet.Channel().Number());
        PrintNumber(print, "Note", packet.Note().Value());
    }
};
}
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Routing ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Routing: una entrada, dos ramas.\nA: acepta solo canal 1. B: reasigna todos al canal 10.\nCanal 1 debe aparecer en A y B; canal 2, solo en B.\nCada entrada incluye Note On y Note Off.\n\n");
    Router<2> router;
    ChannelFilter filter;
    ChannelReassign reassign;
    filter.SetChannels(ChannelMask::Only(Channel::FromNumber(1)));
    reassign.SetTarget(Channel::FromNumber(10));
    router.Output(0)->Bind<ChannelFilter, &ChannelFilter::Process>(&filter);
    router.Output(1)->Bind<ChannelReassign, &ChannelReassign::Process>(&reassign);
    Sink selected{print, "\nRama A: filtro acepto canal 1.\n"};
    Sink moved{print, "\nRama B: reasignado al canal 10.\n"};
    filter.Output().Bind<Sink, &Sink::Process>(&selected);
    reassign.Output().Bind<Sink, &Sink::Process>(&moved);
    const Group group = Group::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    // Channel 1 reaches both branches; channel 2 reaches only the reassigned branch.
    for (int32_t number = 1; number <= 2; ++number) {
        const Channel channel = Channel::FromNumber(number);
        PrintNumber(print, "\nCANAL DE ENTRADA", channel.Number());
        if (number == 2) MIDILAR_EXAMPLE_PRINT(print, "Rama A debe descartar ambos mensajes.\n");
        router.Process(Packet::Midi1NoteOn(group, channel, note, Velocity::FromMidi1(100)));
        router.Process(Packet::Midi1NoteOff(group, channel, note, Velocity::Min()));
    }
}
}
