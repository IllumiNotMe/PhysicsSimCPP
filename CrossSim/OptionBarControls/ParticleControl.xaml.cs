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
using SimEngine;

namespace CrossSim.OptionBarControls
{
    public partial class ParticleControl : UserControl
    {
        private static readonly Random getrandom = new Random();

        public ParticleControl()
        {
            InitializeComponent();
        }

        public ParticleProperties GetParticleProperties()
        {
            ParticleProperties properties = new ParticleProperties
            {
                radius = 10,
                color = ParticleColorPicker.SelectedColor ?? Colors.Red,
                xVel = VelXUpDown.Value ?? 0,
                yVel = VelYUpDown.Value ?? 0,
                mass = MassUpDown.Value ?? 1
            };

            if(RandomColorCheckBox.IsChecked ?? false)
            {
                ParticleColorPicker.R = (byte)getrandom.Next(1, 256);
                ParticleColorPicker.G = (byte)getrandom.Next(1, 256);
                ParticleColorPicker.B = (byte)getrandom.Next(1, 256);
            }

            return properties;
        }

        // Update velocity controls
        private bool changingVel = false;
        private void SliderUpdateVelocity(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (changingVel) return;
            changingVel = true;

            VelXUpDown.Value = XVelocitySlider.Value;
            VelYUpDown.Value = YVelocitySlider.Value;
            UpdateVelBallPosition();

            changingVel = false;
        }
        private void NumBoxUpdateVelocity(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (changingVel) return;
            if (VelXUpDown is null || VelYUpDown is null) return;
            if (XVelocitySlider is null || YVelocitySlider is null) return;

            changingVel = true;

            XVelocitySlider.Value = VelXUpDown.Value ?? 0;
            YVelocitySlider.Value = VelYUpDown.Value ?? 0;

            if (VelocityDisplayBall != null) UpdateVelBallPosition();

            changingVel = false;
        }
        private void UpdateVelBallPosition()
        {
            Thickness ballPosition = new Thickness(0)
            {
                Bottom = YVelocitySlider.Value / 20,
                Left = XVelocitySlider.Value / 20
            };

            VelocityDisplayBall.Margin = ballPosition;
            VelocityArrow.X1 = XVelocitySlider.Value / 40 + 37.5;
            VelocityArrow.Y1 = (-1 * YVelocitySlider.Value / 40 + 37.5);
        }

        // Update color controls
        private void ParticleColorPicker_SelectedColorChanged(object sender, RoutedPropertyChangedEventArgs<Color?> e)
        {
            Brush selectedColor = new SolidColorBrush(ParticleColorPicker.SelectedColor ?? Colors.Red);
            ColorDisplay.Fill = selectedColor;
            VelocityDisplayBall.Fill = selectedColor;
            VelocityArrow.Stroke = selectedColor;
        }

        // Update mass controls
        private bool changingMass = false;
        private void MassSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (changingMass) return;
            if (MassUpDown is null) return;
            changingMass = true;

            MassUpDown.Value = Math.Round(MassSlider.Value, 1);

            changingMass = false;
        }
        private void MassUpDown_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (changingMass || MassSlider is null) return;
            changingMass = true;

            MassSlider.Value = MassUpDown.Value ?? 1;

            changingMass = false;
        }
    }
}
