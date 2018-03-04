using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
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
    enum OptionBarTypes
    {
        ParticleControl,
        LinearForceControl,
        None
    }

    public partial class MainWindow : Window
    {
        MainEngine simEngine;

        private Dictionary<OptionBarTypes, UserControl> optionBarControls = new Dictionary<OptionBarTypes, UserControl>();

        /*
         * Physics constants
        */

        private static readonly double MilliMetersPerWPFUnit = 25.4 / 96.0;

        //Boltzmann constant: k
        private static readonly double boltzConst = 1.38066 * Math.Pow(10, -23);

        // Gas constant: R
        private static readonly double gasConst = 8.314472;

        // Avagodro's number
        private static readonly double avogadroNum = 6.022 * Math.Pow(10, 23);

        // AMU to kilograms
        private static readonly double AMUToKilo = 1.6605 * Math.Pow(10, -27);

        // Physics properties to keep track of
        private int numParticles = 0;
        private double totalEnergy = 0;
        private double totalMomentum = 0;
        private double totalMass = 0;

        // Initialize program
        public MainWindow()
        {
            InitializeComponent();

            // Add render event to be used by physics engine
            CompositionTarget.Rendering += RenderGraphics;

            // Add custom toolbar controls
            optionBarControls[OptionBarTypes.ParticleControl] = new OptionBarControls.ParticleControl();
            optionBarControls[OptionBarTypes.LinearForceControl] = new OptionBarControls.LinearForceControl();

            // Initialize physics engine
            simEngine = new MainEngine(SimGrid, ConsoleTextBox);

            // Update physics info
            UpdatePhysicsProps();
        }


        /*
         * Physics functions
         */

        // Main render loop
        DateTime lastTime = DateTime.Now;
        private void RenderGraphics(object sender, object e)
        {
            DateTime now = DateTime.Now;
            double deltaT = (now - lastTime).TotalSeconds;
            lastTime = now;

            double timeStep;

            if (AutoTimeStepCheckBox.IsChecked ?? true)
            {
                timeStep = deltaT;
                TimeStepEntry.Value = deltaT * 1000.0;
            }
            else
            {
                timeStep = (TimeStepEntry.Value / 1000.0) ?? deltaT;
            }

            if (!(bool)PausePlayButton.IsChecked)
            {
                FPSLabel.Content = Math.Round(1 / deltaT, 0);
                simEngine.Render(timeStep);
            }
        }

        // Particle handling
        private void SpawnParticle(Point spawnPos)
        {
            // Define particle properties
            ParticleProperties pProperties = ((OptionBarControls.ParticleControl)optionBarControls[OptionBarTypes.ParticleControl]).GetParticleProperties();
            pProperties.xPos = spawnPos.X;
            pProperties.yPos = SimGrid.ActualHeight - spawnPos.Y;

            // Spawn particle
            simEngine.SpawnParticle(pProperties);

            // Update physics + generic properties
            numParticles += 1;

            totalMass += pProperties.mass;

            // Velocity magnitude in meters per second
            double velMagnitudeSqr = (Math.Pow(pProperties.xVel, 2) + Math.Pow(pProperties.yVel, 2)) * Math.Pow(MilliMetersPerWPFUnit, 2) / 1000000;
            // Convert from AMU
            double massInKilograms = pProperties.mass * AMUToKilo;

            totalEnergy += 0.5 * (massInKilograms) * velMagnitudeSqr;

            totalMomentum += massInKilograms * Math.Sqrt(velMagnitudeSqr);

            UpdatePhysicsProps();
        }

        // Forces
        private void SpawnLinearForce()
        {
            Tuple<double, double> linearForce = ((OptionBarControls.LinearForceControl)optionBarControls[OptionBarTypes.LinearForceControl]).GetForce();
            simEngine.SpawnLinearForce(linearForce.Item1, linearForce.Item2);
        }

        /*
         * UI Controls
        */

        // Handle click on canvas
        private void SimGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (ParticleToolButton.IsChecked ?? false)
            {
                SpawnParticle(e.GetPosition(SimGrid));
            }
            else if (LinearFToolButton.IsChecked ?? false)
            {
                SpawnLinearForce();
            }
        }

        // Clear sim state
        private void ClearAllButton_Click(object sender, RoutedEventArgs e)
        {
            simEngine.ClearAllParticles();
            simEngine.ClearAllForces();
            numParticles = 0;
            totalEnergy = 0;
            totalMomentum = 0;
            totalMass = 0;
            UpdatePhysicsProps();
        }

        // Update Physics Properties info
        private void UpdatePhysicsProps()
        {
            NumParticlesLabel.Content = String.Format("{0}", numParticles);
            TotalEnergyLabel.Content = String.Format("{0:0.000E0} J", totalEnergy);
            TotalMomentumLabel.Content = String.Format("{0:0.000E0} kg*m/s", totalMomentum);

            // Equation for temperature from average kinetic energy: KE = (3/2) * k * T
            // Solve for T: T = (2 * KE) / (3 * k)
            double avgKE = totalEnergy / numParticles;
            double temp = (2.0 * avgKE) / (3.0 * boltzConst);

            if (Double.IsNaN(temp)) temp = 0;

            TemperatureLabel.Content = String.Format("{0:0##.00000} C", temp - 273.15);

            // Volume meters^2
            double volume = SimGrid.ActualHeight * SimGrid.ActualWidth * Math.Pow(MilliMetersPerWPFUnit, 2) / 1000000;

            double mols = numParticles / avogadroNum;

            // Ideal gas law: PV = nRT where pressure is in Pascals (N*m^-2)
            // Pressure = (nRT) / V
            double pressure = (mols * gasConst * temp) / volume;
            PressureLabel.Content = String.Format("{0:0.00000E0} Pa", pressure);
        }

        // Toolbar logic
        ToggleButton lastTool;
        private void ToolSelected(object sender, RoutedEventArgs e)
        {
            if (sender == lastTool) return;

            // Make sure only one tool is selected
            foreach (UIElement elem in ToolSelectButtons.Children)
            {
                if (elem is ToggleButton && elem != sender)
                {
                    ((ToggleButton)elem).IsChecked = false;
                }

                ((ToggleButton)sender).IsChecked = true;
                lastTool = (ToggleButton)sender;
            }

            // Close last tool's options
            if (OptionBar.Children.Count > 0)
                OptionBar.Children.RemoveRange(0, OptionBar.Children.Count);

            // Open appropriate customization window
            OptionBarTypes barType = OptionBarTypes.None;

            if (ParticleToolButton.IsChecked ?? false)
                barType = OptionBarTypes.ParticleControl;

            else if (LinearFToolButton.IsChecked ?? false)
                barType = OptionBarTypes.LinearForceControl;

            if (barType != OptionBarTypes.None)
                OptionBar.Children.Add(optionBarControls[barType]);
        }

        // Force shutdown the application - closes all child windows
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Application.Current.Shutdown();
        }

        // Open and close engine settings window
        private void EngineSettingsButtonClick(object sender, RoutedEventArgs e)
        {
            if (EnginePropertiesWindow.IsVisible)
            {
                EnginePropertiesWindow.Close();
            }
            else
            {
                EnginePropertiesWindow.Show();
            }
        }
    }
}
