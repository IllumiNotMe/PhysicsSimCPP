using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Windows.Controls;

namespace CrossSim
{
    class LineTrail
    {
        public Polyline trail;
        private Shape trailSource;
        private Grid container;
        
        public bool longEnough = false;
        public double numFrames;

        double sourceRadius;

        public LineTrail(Shape trailSource, Grid container, int numFrames)
        {
            this.container = container;
            this.numFrames = numFrames;
            this.trailSource = trailSource;
            sourceRadius = trailSource.Height / 2;

            trail = new Polyline()
            {
                Stroke = trailSource.Fill.Clone(),
                StrokeThickness = 2
            };

            container.Children.Add(trail);
        }

        public void Update()
        {
            trail.Points.Add(new Point(trailSource.Margin.Left + sourceRadius, container.ActualHeight - trailSource.Margin.Bottom - sourceRadius));

            if (longEnough)
            {
                trail.Points.RemoveAt(0);
            }
            else if (trail.Points.Count >= numFrames)
            {
                longEnough = true;
            }
        }
    }
}
