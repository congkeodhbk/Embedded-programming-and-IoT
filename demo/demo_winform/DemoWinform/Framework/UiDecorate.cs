using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
namespace Framework
{
    public static class UiDecorate
    {
        static void boolToColor(object sender, ConvertEventArgs cevent)
        {
            // The method converts only to string type. Test this using the DesiredType.
            if (cevent.DesiredType != typeof(Color)) return;

            // Use the ToString method to format the value as currency ("c").
            cevent.Value = ((bool)cevent.Value) ? Color.LightGreen : Color.Salmon;
        }
        static private void stringToColorOK(object sender, ConvertEventArgs cevent)
        {
            // The method converts only to string type. Test this using the DesiredType.
            if (cevent.DesiredType != typeof(Color)) return;

            // Use the ToString method to format the value as currency ("c").
            cevent.Value = ((string)cevent.Value).Equals("OK") ? Color.LightGreen : Color.Salmon;
        }
        static private void stringNotNullToColor(object sender, ConvertEventArgs cevent)
        {
            // The method converts only to string type. Test this using the DesiredType.
            if (cevent.DesiredType != typeof(Color)) return;

            // Use the ToString method to format the value as currency ("c").
            cevent.Value = string.IsNullOrEmpty((string)cevent.Value) ? Color.Salmon : Color.LightGreen;
        }
        static private void stringZeroToColor(object sender, ConvertEventArgs cevent)
        {
            // The method converts only to string type. Test this using the DesiredType.
            if (cevent.DesiredType != typeof(Color)) return;

            // Use the ToString method to format the value as currency ("c").
            cevent.Value = (string.IsNullOrEmpty((string)cevent.Value) ||string.Equals((string)cevent.Value,"0")) ? Color.Salmon : Color.LightGreen;
        }
        public static void addTextOkToColorBinding(ref Label lb)
        {
            Binding bind = new Binding("BackColor", lb, "Text");
            bind.Format += stringToColorOK;
            lb.DataBindings.Add(bind);
        }
        public static void addTextNotNullToColorBinding(ref Label tb)
        {
            Binding bind = new Binding("BackColor", tb, "Text");
            bind.Format += stringNotNullToColor;
            tb.DataBindings.Add(bind);
        }
        public static void addTextZeroToColorBinding(ref Label tb)
        {
            Binding bind = new Binding("BackColor", tb, "Text");
            bind.Format += stringZeroToColor;
            tb.DataBindings.Add(bind);
        }
        public static void addBoolToColorBinding(ref CheckBox cb)
        {
            Binding bind = new Binding("BackColor", cb, "Checked");
            bind.Format += boolToColor;
            cb.DataBindings.Add(bind);
        }
    }
}
