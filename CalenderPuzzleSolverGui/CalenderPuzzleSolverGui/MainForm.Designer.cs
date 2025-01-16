namespace CalenderPuzzleSolverGui
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            buttonSolve = new Button();
            buttonSolveToday = new Button();
            buttonClear = new Button();
            SuspendLayout();
            // 
            // buttonSolveToday
            // 
            buttonSolveToday.Location = new Point(490, 820);
            buttonSolveToday.Name = "buttonSetCurrentDate";
            buttonSolveToday.Size = new Size(200, 70);
            buttonSolveToday.Text = "Solve Today";
            buttonClear.TabIndex = 0;
            buttonSolveToday.UseVisualStyleBackColor = true;
            buttonSolveToday.Click += SolveToday;
            // 
            // buttonSolve
            // 
            buttonSolve.Location = new Point(250, 820);
            buttonSolve.Name = "buttonSolve";
            buttonSolve.Size = new Size(200, 70);
            buttonSolve.Text = "Solve";
            buttonSolve.UseVisualStyleBackColor = true;
            buttonSolve.Click += Solve;
            // 
            // buttonClear
            // 
            buttonClear.Location = new Point(10, 820);
            buttonClear.Name = "buttonClear";
            buttonClear.Size = new Size(200, 70);
            buttonClear.Text = "Clear";
            buttonClear.TabIndex = 1;
            buttonClear.UseVisualStyleBackColor = true;
            buttonClear.Click += Clear;
            // 
            // MainForm
            // 
            BackColor = SystemColors.Control;
            ClientSize = new Size(700, 900);
            Controls.Add(buttonSolveToday);
            Controls.Add(buttonSolve);
            Controls.Add(buttonClear);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Name = "MainForm";
            Text = "Puzzle Solver for Stupid Lacy";
            ResumeLayout(false);
        }

        #endregion
        private Button buttonSolve;
        private Button buttonSolveToday;
        private Button buttonClear;
    }
}
