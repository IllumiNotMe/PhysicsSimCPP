using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using System.Diagnostics;

namespace CrossSim
{
    class ParticleTrail
    {
        Ellipse trail;
        Grid container;

        DateTime creationTime;
        double timeout;

        double trailWidth = 5;

        public ParticleTrail(Shape trailSource, Grid container, double timeout)
        {
            this.container = container;
            this.timeout = timeout;

            double radius = trailWidth / 2;

            // Create ellipse for trail
            trail = new Ellipse()
            {
                Height = trailWidth,
                Width = trailWidth,
                Fill = trailSource.Fill.Clone(),

                VerticalAlignment = VerticalAlignment.Bottom,
                HorizontalAlignment = HorizontalAlignment.Left
            };

            // Set position
            Thickness position = trailSource.Margin;
            position.Bottom += (trailSource.Height / 2) - radius;
            position.Left += (trailSource.Width / 2) - radius;
            trail.Margin = position;

            // Add to SimGrid
            container.Children.Add(trail);

            // Track creation time
            creationTime = DateTime.Now;
        }

        public bool Update()
        {
            double timeAlive = (DateTime.Now - creationTime).TotalSeconds;

            double deathPercent = timeAlive / timeout;

            if (timeAlive >= timeout)
            {
                FadeOut();
                return false;
            }
            else
            {
                trail.Fill.Opacity = 1 - deathPercent;
                return true;
            }
        }

        public void FadeOut()
        {
            container.Children.Remove(trail);
        }
    }
}