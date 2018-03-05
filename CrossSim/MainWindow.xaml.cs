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
        private List<Particle> currentParticles = new List<Particle>();
        private List<Tuple<double, double>> currentForces = new List<Tuple<double, double>>();

        /*
         * Physics constants
        */

        //Boltzmann constant: k
        private static readonly double boltzConst = 1.38066 * Math.Pow(10, -23);

        // Gas constant: R
        private static readonly double gasConst = 8.314472;

        // Avagodro's number
        private static readonly double avogadroNum = 6.022 * Math.Pow(10, 23);

        // AMU to kilograms
        private static readonly double AMUToKilo = 1.6605 * Math.Pow(10, -27);

        // Physics properties to keep track of
        private double totalEnergy = 0;
        private double totalMomentum = 0;
        private double totalMass = 0;
        private double windowVolume = 0;

        // Initialize program
        public MainWindow()
        {
            InitializeComponent();

            // Add render event to be used by physics engine
            CompositionTarget.Rendering += RenderGraphics;
            KeyDown += HandleKeyPress;

            // Add custom toolbar controls
            optionBarControls[OptionBarTypes.ParticleControl] = new OptionBarControls.ParticleControl();

            OptionBarControls.LinearForceControl linearForceControl = new OptionBarControls.LinearForceControl();
            linearForceControl.AddForceButton.Click += (s, e) => { SpawnLinearForce(); };
            optionBarControls[OptionBarTypes.LinearForceControl] = linearForceControl;

            // Initialize physics engine
            simEngine = new MainEngine(SimGrid, ConsoleTextBox);

            // Update physics info
            UpdatePhysicsProps();

            // Select default tool
            SelectTool(ParticleToolButton);
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

                UpdateParticleInfoDisplay();
            }
        }

        // Particle handling
        private void SpawnParticle(Point spawnPos)
        {
            // Define particle properties
            ParticleProperties pProperties = ((OptionBarControls.ParticleControl)optionBarControls[OptionBarTypes.ParticleControl]).GetParticleProperties();
            pProperties.xPos = spawnPos.X;
            pProperties.yPos = SimGrid.ActualHeight - spawnPos.Y;

            // Add particle to list view
            if (((ListViewItem)ParticleListView.Items[0]).Content as string == "None")
            {
                ParticleListView.Items.RemoveAt(0);
            }

            StackPanel itemContent = new StackPanel() { Orientation = Orientation.Horizontal };
            itemContent.Children.Add(new TextBlock() { Text = "Particle " + currentParticles.Count.ToString() });
            itemContent.Children.Add(new Ellipse()
            {
                Fill = new SolidColorBrush(pProperties.color),
                Width = 10,
                Height = 10,
                Margin = new Thickness(5, 0, 5, 0)
            });

            ParticleListView.Items.Add(new ListViewItem() { Content = itemContent });

            // Update physics + generic properties
            totalMass += pProperties.mass;

            // Velocity magnitude in meters per second
            double velMagnitudeSqr = Math.Pow(pProperties.xVel, 2) + Math.Pow(pProperties.yVel, 2);
            // Convert from AMU
            double massInKilograms = pProperties.mass * AMUToKilo;

            totalEnergy += massInKilograms * velMagnitudeSqr / 2.0;

            totalMomentum += massInKilograms * Math.Sqrt(velMagnitudeSqr);

            UpdatePhysicsProps();

            // Edit pProperties for engine and spawn particle
            pProperties.xVel *= 0.1;
            pProperties.yVel *= 0.1;

            Particle newParticle = simEngine.SpawnParticle(pProperties);
            currentParticles.Add(newParticle);
        }

        // Forces
        private void SpawnLinearForce()
        {
            Tuple<double, double> linearForce = ((OptionBarControls.LinearForceControl)optionBarControls[OptionBarTypes.LinearForceControl]).GetForce();
            simEngine.SpawnLinearForce(linearForce.Item1, linearForce.Item2);

            // Add particle to list view
            if (((ListViewItem)ForcesListView.Items[0]).Content as string == "None")
            {
                ForcesListView.Items.RemoveAt(0);
            }

            StackPanel itemContent = new StackPanel();
            itemContent.Children.Add(new TextBlock() { Text="Linear Force" });

            ForcesListView.Items.Add(new ListViewItem() { Content=itemContent });

            currentForces.Add(linearForce);
        }

        /*
         * UI Controls
        */

        // Handle KeyPress
        bool windowFullscreen = false;
        private void HandleKeyPress(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.F11)
            {
                if (windowFullscreen)
                {
                    WindowState = WindowState.Normal;
                    WindowStyle = WindowStyle.SingleBorderWindow;
                    windowFullscreen = false;
                }
                else
                {
                    WindowState = WindowState.Maximized;
                    WindowStyle = WindowStyle.None;
                    windowFullscreen = true;
                }
            }
        }

        // Handle click on canvas
        private void SimGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (ParticleToolButton.IsChecked ?? false)
            {
                SpawnParticle(e.GetPosition(SimGrid));
            }
        }

        // Clear sim state
        private void ClearAllButton_Click(object sender, RoutedEventArgs e)
        {
            simEngine.ClearAllParticles();
            simEngine.ClearAllForces();

            while (ParticleListView.Items.Count > 0)
                ParticleListView.Items.RemoveAt(0);
            ParticleListView.Items.Add(new ListViewItem() { Content = "None" });

            while (ForcesListView.Items.Count > 0)
                ForcesListView.Items.RemoveAt(0);
            ForcesListView.Items.Add(new ListViewItem() { Content = "None" });

            currentParticles = new List<Particle>();
            currentForces = new List<Tuple<double, double>>();

            totalEnergy = 0;
            totalMomentum = 0;
            totalMass = 0;
            UpdatePhysicsProps();
        }

        // Update Physics Properties info
        private void UpdatePhysicsProps()
        {
            int numParticles = currentParticles.Count + 1;

            NumParticlesLabel.Content = String.Format("{0}", numParticles);
            TotalEnergyLabel.Content = String.Format("{0:0.000E0} J", totalEnergy);
            TotalMomentumLabel.Content = String.Format("{0:0.000E0} kg*m/s", totalMomentum);

            // Equation for temperature from average kinetic energy: KE = (3/2) * k * T
            // Solve for T: T = (2 * KE) / (3 * k)
            double avgKE = totalEnergy / numParticles;
            double temp = (2.0 * avgKE) / (3.0 * boltzConst);

            if (Double.IsNaN(temp)) temp = 0;

            TemperatureLabel.Content = String.Format("{0:0##.00000} C", temp - 273.15);

            double mols = numParticles / avogadroNum;

            // Ideal gas law: PV = nRT where pressure is in Pascals (N*m^-2)
            // Pressure = (nRT) / V
            double pressure = (mols * gasConst * temp) / windowVolume;

            if (double.IsNaN(pressure)) pressure = 0;

            PressureLabel.Content = String.Format("{0:0.00000E0} Pa", pressure);
        }

        // Toolbar logic
        ToggleButton lastTool;
        private void ToolSelected(object sender, RoutedEventArgs e)
        {
            if (sender == lastTool)
            {
                lastTool.IsChecked = true;
                return;
            }

            SelectTool((ToggleButton)sender);
        }
        private void SelectTool(ToggleButton tool)
        {
            // Make sure only one tool is selected
            foreach (UIElement elem in ToolSelectButtons.Children)
            {
                if (elem is ToggleButton && elem != tool)
                {
                    ((ToggleButton)elem).IsChecked = false;
                }

                tool.IsChecked = true;
                lastTool = tool;
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

        // Update time step controls on checkbox changed
        private void AutoTimeStepCheckBox_Changed(object sender, RoutedEventArgs e)
        {
            bool? isChecked = (sender as CheckBox).IsChecked;
            if (isChecked is null) return;
            if (TimeStepEntry is null) return;

            TimeStepEntry.IsEnabled = !(bool)isChecked;
        }

        // Updates window volume
        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            // Converts from grid units to meters
            double windowHeight = e.NewSize.Height * 10;
            double windowWidth = e.NewSize.Width * 10;
            windowVolume = windowWidth * windowHeight;

            WindowSizeLabel.Content = String.Format("{0:0.##}m x {1:0.##}m", windowWidth, windowHeight);

            UpdatePhysicsProps();
        }

        // Update particle info list
        Particle selectedParticle = null;
        private void UpdateParticleInfoDisplay()
        {
            if (selectedParticle is null) return;

            ParticleMassLabel.Content = String.Format("{0:0.##}", selectedParticle.mass());
            ParticleVelocityLabel.Content = String.Format("{0:0.##}x, {1:0.##}y", selectedParticle.velX * 10.0, selectedParticle.velY * 10.0);
        }
        private void ParticleListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            // If nothing is selected, exit
            int pIndex = ParticleListView.SelectedIndex;
            if (pIndex == -1 || currentParticles.Count == 0) return;

            // If the same thing is selected again, exit
            if (selectedParticle == currentParticles[pIndex]) return;

            // Release last selected particle
            if (selectedParticle != null)
                selectedParticle.shape.shape.StrokeThickness = 0;

            // Update selected particle
            selectedParticle = currentParticles[pIndex];
            selectedParticle.shape.shape.Stroke = new SolidColorBrush(Colors.Black);
            selectedParticle.shape.shape.StrokeThickness = 2;
        }

        // Update forces info list
        private void ForcesListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int fIndex = ForcesListView.SelectedIndex;
            if (fIndex == -1 || currentForces.Count == 0) return;

            ForceMagnitudeLabel.Content = String.Format("{0:0.##}x, {1:0.##}y", currentForces[fIndex].Item1, currentForces[fIndex].Item2);
        }
    }
}
