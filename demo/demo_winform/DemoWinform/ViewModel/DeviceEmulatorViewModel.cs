using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Interfaces;
using Framework;
namespace ViewModel
{
    public class DeviceEmulatorViewModel
    {
        public ISerialController serialControler { get => serialMonitor; }
        public ISerialSlipObserver serialSlip { get => serialMonitor; }
        private SerialMonitor serialMonitor;
        public DeviceEmulatorViewModel()
        {
            serialMonitor = new SerialMonitor();
        }
    }
}
