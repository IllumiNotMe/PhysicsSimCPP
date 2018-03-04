using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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

namespace CrossSim.OptionBarControls
{
    /// <summary>
    /// Interaction logic for LinearForceControl.xaml
    /// </summary>
    public partial class LinearForceControl : UserControl
    {
        public LinearForceControl()
        {
            InitializeComponent();
        }

        private void GravityButton_Click(object sender, RoutedEventArgs e)
        {
            MagXInput.Value = 0;
            MagYInput.Value = -9.80665;
        }

        public Tuple<double, double> GetForce()
        {
            Tuple<double, double> force = new Tuple<double, double>(
                MagXInput.Value ?? 0,
                MagYInput.Value ?? 0
                );
            return force;
        }
    }
}
