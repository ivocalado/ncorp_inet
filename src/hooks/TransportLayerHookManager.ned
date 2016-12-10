//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package ncorp.hooks;

//
// TODO auto-generated type
//
module TransportLayerHookManager
{
    parameters:
        @display("i=block/fork;bgb=848,286");
        string hookType = default("");
        bool hasHook = default(hookType != "");
    gates:
        input transportIn[] @labels(ITransportPacket/down);
        output transportOut[] @labels(ITransportPacket/up);
        input networkIn[] @labels(INetworkDatagram/up);
        output networkOut[] @labels(INetworkDatagram/down);
    submodules:
        hook: <hookType> like IHookLayer if hasHook {
            parameters:
                ports = sizeof(transportIn);
        }
    connections allowunconnected:
        for i=0..sizeof(transportIn)-1 {
            transportIn[i] --> networkOut[i] if !hasHook;
            transportOut[i] <-- networkIn[i] if !hasHook;

            transportIn[i] --> hook.upIn[i] if hasHook;
            transportOut[i] <-- hook.upOut[i] if hasHook;

            hook.downIn[i] <-- networkIn[i] if hasHook;
            hook.downOut[i] --> networkOut[i] if hasHook;
        }

}
