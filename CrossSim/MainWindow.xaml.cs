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
using System.Diagnostics;
using SimEngine;

namespace CrossSim
{
    public partial class MainWindow : Window
    {
        MainEngine simEngine;

        public MainWindow()
        {
            InitializeComponent();
            CompositionTarget.Rendering += RenderGraphics;

            simEngine = new MainEngine(SimGrid);
        }

        DateTime lastTime = DateTime.Now;
        private void RenderGraphics(object sender, object e)
        {
            DateTime now = DateTime.Now;
            double deltaT = (now - lastTime).TotalSeconds;
            lastTime = now;

            simEngine.Render(deltaT);
        }

        private void SimGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point clickPos = e.GetPosition(SimGrid);

            simEngine.SpawnParticle(clickPos.X,
                SimGrid.ActualHeight - clickPos.Y, // Convert top-left anchored coords to bottom-left coords
                10,
                (Color)MaterialColorPicker.SelectedColor);
        }
    }
}
