using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ViewModel;
namespace DeviceEmulator
{
    public partial class DeviceEmulator : Form
    {
        private DeviceEmulatorViewModel _vm = new DeviceEmulatorViewModel();
        public DeviceEmulator()
        {
            InitializeComponent();
            button1.Click += delegate { _vm.serialControler.Execute(comboBox1.Text); };
            button1.DataBindings.Add(_vm.serialControler.ControlTextBinding);
            comboBox1.DataBindings.Add(_vm.serialControler.PortSelectEnabledBinding);
            comboBox1.DataSource = _vm.serialControler.PortList;
        }
    }
}
