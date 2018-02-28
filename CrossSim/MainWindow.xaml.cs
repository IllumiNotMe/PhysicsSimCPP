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

        // Initialize program
        public MainWindow()
        {
            InitializeComponent();
            CompositionTarget.Rendering += RenderGraphics;

            simEngine = new MainEngine(SimGrid, ConsoleTextBox);
        }

        // Main render loop
        DateTime lastTime = DateTime.Now;
        private void RenderGraphics(object sender, object e)
        {
            DateTime now = DateTime.Now;
            double deltaT = (now - lastTime).TotalSeconds;
            lastTime = now;

            if (!(bool)PausePlayButton.IsChecked)
            {
                FPSLabel.Content = Math.Round(1 / deltaT, 0);
                simEngine.Render(deltaT);
            }
        }

        // Spawn particle upon left click on canvas
        private void SimGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point clickPos = e.GetPosition(SimGrid);

            ParticleProperties pProperties = new ParticleProperties()
            {
                xPos = clickPos.X,
                yPos = SimGrid.ActualHeight - clickPos.Y,
                radius = 10,
                color = (Color)MaterialColorPicker.SelectedColor,
                xVel = -1 * XVelocitySlider.Value,
                yVel = YVelocitySlider.Value
            };

            simEngine.SpawnParticle(pProperties);

            /*
            simEngine.SpawnParticle(clickPos.X,
                SimGrid.ActualHeight - clickPos.Y, // Convert top-left anchored coords to bottom-left coords
                10,
                (Color)MaterialColorPicker.SelectedColor);*/
        }

        // Update velocity flyout
        private void FormatVelocityDisplay(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            VelocityLabel.Content = String.Format("({0:0.#}, {1:0.#})", XVelocitySlider.Value * -1, YVelocitySlider.Value);

            Thickness ballPosition = new Thickness(0)
            {
                Bottom = YVelocitySlider.Value,
                Left = XVelocitySlider.Value
            };

            VelocityDisplayBall.Margin = ballPosition;
            VelocityArrow.X1 = XVelocitySlider.Value * (.5) + 37.5;
            VelocityArrow.Y1 = (-1 * YVelocitySlider.Value * (.5) + 37.5);
        }
    }
}
