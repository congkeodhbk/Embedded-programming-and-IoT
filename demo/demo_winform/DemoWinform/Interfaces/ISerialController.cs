using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel;
namespace Interfaces
{
    public interface ISerialController
    {
        Binding PortSelectEnabledBinding { get; }
        Binding ControlTextBinding { get; }
        BindingList<string> PortList { get; }
        void Execute(string port);
    }
}
