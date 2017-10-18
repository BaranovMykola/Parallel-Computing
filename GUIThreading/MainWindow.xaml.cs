using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GUIThreading
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private int _progValue = 0;

        public int ProgValue
        {
            get { return _progValue; }
            set
            {
                _progValue = value; 
                OnPropertyChanged("ProgValue");
            }
        }

        public CancellationToken token { get; set; }

        public Task t { get; set; }

        public CancellationTokenSource tokenSource { get; set; }

        public bool Canceled { get; set; } = false;

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            Canceled = false;
            t = Task.Run(() =>
            {
                for (; ProgValue < 1000 && !Canceled; ProgValue++)
                {
                    Thread.Sleep(10);
                }
            }, token);
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void ButtonStop_OnClick(object sender, RoutedEventArgs e)
        {
            Canceled = true;
        }

        private void ButtonNew_OnClick(object sender, RoutedEventArgs e)
        {
            var win = new MainWindow();
            win.Show();
        }
    }
}
