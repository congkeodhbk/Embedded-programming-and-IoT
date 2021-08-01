using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Runtime.CompilerServices;
namespace Framework
{
    [Serializable]
    public class Bindable: INotifyPropertyChanged
    {
        [field: NonSerialized]
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void Notify([CallerMemberName] string property = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
        protected virtual void Set<T>(ref T member, T value, [CallerMemberName] string property = null)
        {
            if (Equals(member, value))
                return;
            member = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
        protected virtual void SetValueWithColor<T>(ref T member, T value, [CallerMemberName] string property = null)
        {
            if (Equals(member, value))
                return;
            member = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
        protected virtual void Set<T>(ref T member, T value, string[] properties, [CallerMemberName] string property = null)
        {
            if(Equals(member, value))
            {
                return;
            }
            member = value;
            if (properties.Length <= 0) return;
            foreach(var p in properties)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(p));
            }
        }

        protected virtual void Set<T>(ref T member, T value, Predicate<T> predicate, string message, [CallerMemberName] string property = null)
        {
            if (Equals(member, value))
            {
                return;
            }
            if (!predicate(value))
                throw new Exception(message);
            member = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
    }
}
