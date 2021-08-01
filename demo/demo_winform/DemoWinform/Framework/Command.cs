using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace Framework
{
    public class Command<T>: Bindable
    {
        private readonly Action<T> _action;
        private readonly Func<bool> _predicate;
        public bool Enabled => _predicate == null ? true : _predicate.Invoke();
        public Binding EnabledBinding { get; private set; }
        public Command(Action<T>action, Func<bool> predicate)
        {
            _action = action;
            _predicate = predicate;
            EnabledBinding = new Binding("Enabled", this, nameof(Enabled));
        }
        public Command(Action<T>action): this(action, null) { }
        public void Execute(T t) => _action.Invoke(t);
        public void NotifyChange(string property = nameof(Enabled))
        {
            Notify(property);
        }
    }
    public class Command: Bindable
    {
        private readonly Action _action;
        private readonly Func<bool> _predicate;
        public bool Enabled => _predicate == null ? true : _predicate.Invoke();
        public Binding EnabledBinding { get; private set; }
        public Command(Action action, Func<bool> predicate)
        {
            _action = action;
            _predicate = predicate;
            EnabledBinding = new Binding("Enabled", this, nameof(Enabled));
        }
        public void Execute() => _action.Invoke();
        public void NotifyChange(string property = nameof(Enabled))
        {
            Notify(property);
        }
    }
}
