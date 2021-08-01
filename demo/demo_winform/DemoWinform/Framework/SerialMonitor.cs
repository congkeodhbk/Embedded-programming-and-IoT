using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.ComponentModel;
using Interfaces;
namespace Framework
{
    public class SerialMonitor:Bindable, ISerialController, ISerialSlipObserver
    {
        public bool Connected
        {
            get => _connected;
            set => Set(ref _connected, value, new string[] { nameof(ControlText), nameof(PortSelectEnabled) });
        }

        //if serial is connected, disable combobox, button text change to close.
        public bool PortSelectEnabled => Connected?false:true;
        public string ControlText => Connected? "Close" : "Open";
        public Binding PortSelectEnabledBinding { get; private set; }
        public Binding ControlTextBinding { get; private set; }
        public BindingList<string> PortList { get; private set; }

        //public event ReceivedDataHandler<ISerialSlipObserver> dataReceived;
       
        //private
        private bool _connected;
        private string[] _ports;
        private Timer timer;
        private SerialPort serial;
        private Slip slip;

        public event SerialSlipReceivedHandler slipReceivedHandler;

        public SerialMonitor()
        {
            serial = new SerialPort();
            serial.BaudRate = 115200;
            serial.DataBits = 8;
            serial.Parity = Parity.None;
            serial.Handshake = Handshake.None;
            serial.StopBits = StopBits.One ;
            serial.DataReceived += DataReceivedHandler;
            slip = new Slip();
            PortSelectEnabledBinding = new Binding("Enabled", this, nameof(PortSelectEnabled));
            ControlTextBinding = new Binding("Text", this, nameof(ControlText));
            timer = new Timer();
            timer.Tick += connectionCheck;
            timer.Interval = 500;
            timer.Enabled = true;
            PortList = new BindingList<string>();
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            try
            {
                while (sp.BytesToRead > 0)
                {
                    int b = sp.ReadByte();
                    List<byte> frame = slip.parse((byte)b);
                    if (frame != null)
                    {
                        slipReceivedHandler?.Invoke(this, new SerialSlipEventArgs(frame));
                    }
                }
                sp.DiscardInBuffer();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        void connectionCheck(object sender, EventArgs e)
        {
            if (Connected && serial.IsOpen)
            {
                return;
            }
            else if(Connected && !serial.IsOpen)
            {
                Connected = false;
            }

            string[] newPort = SerialPort.GetPortNames();
            if (newPort == null && _ports != null)
            {
                _ports = null;
                PortList.Clear();
            }
            else if (newPort != null && _ports == null)
            {
                _ports = newPort;
                PortList.Clear();
                foreach (string port in _ports)
                {
                    PortList.Add(port);
                }
            }
            else
            {
                if (newPort.SequenceEqual(_ports))
                {
                    return;
                }
                PortList.Clear();
                _ports = newPort;
                foreach (string port in _ports)
                {
                    PortList.Add(port);
                }
            }
        }

        public void Execute(string port)
        {
            if (Connected)
            {
                serial.Close();
                Connected = false;
            }
            else if (!string.IsNullOrEmpty(port))
            {
                serial.PortName = port;
                try
                {
                    serial.Open();
                    Connected = true;
                }
                catch
                {

                }
            }
            //Connected = serial.IsOpen;
        }

        public void NotifyChange(string property)
        {
            Notify(property);
        }
        public int send(List<byte> data)
        {
            if (!serial.IsOpen)
                return 0;
            slip.encode(data);
            byte[] _d = data.ToArray();
            serial.Write(_d, 0, _d.Length);
            return _d.Length;
        }
    }
}
