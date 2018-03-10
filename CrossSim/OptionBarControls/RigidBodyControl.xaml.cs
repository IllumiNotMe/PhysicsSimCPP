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
    /// Interaction logic for RigidBodyControl.xaml
    /// </summary>
    public partial class RigidBodyControl : UserControl
    {
        public RigidBodyControl()
        {
            InitializeComponent();
        }

        private void BodyColorPicker_SelectedColorChanged(object sender, RoutedPropertyChangedEventArgs<Color?> e)
        {
            Brush selectedColor = new SolidColorBrush(BodyColorPicker.SelectedColor ?? Colors.Red);
            ColorDisplay.Fill = selectedColor;
        }
    }
}
