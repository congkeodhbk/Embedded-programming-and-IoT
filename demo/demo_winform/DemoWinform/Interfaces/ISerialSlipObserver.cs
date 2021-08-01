using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Interfaces
{
    public delegate void SerialSlipReceivedHandler(object sender, SerialSlipEventArgs args);

    public class SerialSlipEventArgs : EventArgs
    {
        List<byte> data;
        public SerialSlipEventArgs(List<byte> d)
        {
            data = d;
        }
    }
    public interface ISerialSlipObserver
    {
        event SerialSlipReceivedHandler slipReceivedHandler;
        int send(List<byte> data);
    }
}
