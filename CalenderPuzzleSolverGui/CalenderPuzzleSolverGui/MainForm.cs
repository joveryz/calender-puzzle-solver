using System;
using System.Diagnostics;
using System.Text;
using static System.Windows.Forms.LinkLabel;

namespace CalenderPuzzleSolverGui
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            Initialize();
        }

        private Dictionary<int, Button> AllButtons;

        private static Color ButtonDefaultColor = Color.White;

        private static Color ButtonPressedColor = Color.DimGray;

        private static readonly string ButtonTextPlaceHolder = "PlaceHolder";

        private static readonly int ButtonCountPerLine = 7;

        private static readonly int ButtonCountTotal = 56;

        private static bool IsCleared = false;

        private List<string> ButtonTexts =
        [
            "Jan",
            "Feb",
            "Mar",
            "Apr",
            "May",
            "Jun",
            ButtonTextPlaceHolder,
            "Jul",
            "Aug",
            "Sep",
            "Oct",
            "Nov",
            "Dec",
            ButtonTextPlaceHolder,
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
            "Sun",
            "Mon",
            "Tue",
            "Wed",
            ButtonTextPlaceHolder,
            ButtonTextPlaceHolder,
            ButtonTextPlaceHolder,
            ButtonTextPlaceHolder,
            "Thu",
            "Fri",
            "Sat",
        ];

        private Dictionary<char, Color> ColorMap = new Dictionary<char, Color>
        {
            { '0', ButtonPressedColor },
            { '1', Color.Yellow },
            { '2', Color.Violet },
            { '3', Color.Green },
            { '4', Color.Red },
            { '5', Color.Purple },
            { '6', Color.Orange },
            { '7', Color.Blue },
            { '8', Color.Cyan },
            { '9', Color.DeepPink },
            { 'X', Color.YellowGreen },
        };

        private void Initialize()
        {
            AllButtons = new Dictionary<int, Button>();
            int startPosX = 0;
            int startPosY = 0;

            for (int i = 0; i < ButtonCountTotal; i++)
            {
                Button button = new Button();
                button.FlatAppearance.BorderSize = 0;
                button.Location = new Point(startPosX, startPosY);
                button.Margin = new Padding(0);
                button.Size = new Size(100, 100);
                button.Text = ButtonTexts[i];
                button.UseVisualStyleBackColor = true;
                button.BackColor = ButtonDefaultColor;
                button.Click += Click;
                button.Visible = ButtonTexts[i] == ButtonTextPlaceHolder ? false : true;

                AllButtons[i + 1] = button;
                Controls.Add(button);

                startPosX += 100;
                if ((i + 1) % ButtonCountPerLine == 0)
                {
                    startPosX = 0;
                    startPosY += 100;
                }
            }
        }

        private void Click(object sender, EventArgs e)
        {
            if (!IsCleared)
            {
                Clear(sender, e);
            }

            Button button = (Button)sender;
            button.BackColor = button.BackColor == ButtonDefaultColor ? ButtonPressedColor : ButtonDefaultColor;
        }

        private bool ValidateAndGenerateInput()
        {
            int[] count = [0, 0, 0];
            StringBuilder sb = new StringBuilder();
            foreach (var kvp in AllButtons)
            {
                if (kvp.Key != 1 && kvp.Key % ButtonCountPerLine == 1)
                {
                    sb.Append('\n');
                }

                if (!kvp.Value.Visible)
                {
                    sb.Append('0');
                    continue;
                }

                if (kvp.Value.BackColor == ButtonPressedColor)
                {
                    if (kvp.Key <= 14)
                    {
                        count[0]++;
                    }
                    else if (kvp.Key > 14 && kvp.Key <= 45)
                    {
                        count[1]++;
                    }
                    else
                    {
                        count[2]++;
                    }
                    sb.Append('0');
                }
                else
                {
                    sb.Append('.');
                }
            }
            if (count[0] == 1 && count[1] == 1 && count[2] == 1)
            {
                File.WriteAllText("input.txt", sb.ToString());
                return true;
            }
            else
            {
                return false;
            }
        }

        private void Cleanup()
        {
            File.Delete("input.txt");
            File.Delete("output.txt");
        }

        private void Solve(object sender, EventArgs e)
        {
            Cleanup();
            if (!ValidateAndGenerateInput())
            {
                MessageBox.Show("Input is not valid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Process process = new Process();
            process.StartInfo.FileName = "PuzzleSolver.exe";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            process.Start();
            process.WaitForExit();

            var output = File.ReadAllLines("output.txt").ToList();

            for (int i = 0; i < output.Count(); i++)
            {
                for (int j = 0; j < output[i].Length; j++)
                {
                    int index = i * ButtonCountPerLine + j + 1;
                    if (AllButtons[index].Visible)
                    {
                        AllButtons[index].BackColor = ColorMap[output[i][j]];
                    }
                }
            }

            IsCleared = false;
            Cleanup();
        }

        private void SolveToday(object sender, EventArgs e)
        {
            Clear(sender, e);

            var now = DateTime.Now;
            var indexMonth = now.Month;
            var indexDay = now.Day + 14;
            var indexWeek = (int)now.DayOfWeek;

            if (indexMonth > 6)
            {
                indexMonth++;
            }

            if (indexWeek < 4)
            {
                indexWeek += 46;
            }
            else
            {
                indexWeek += 50;
            }

            AllButtons[indexMonth].BackColor = ButtonPressedColor;
            AllButtons[indexDay].BackColor = ButtonPressedColor;
            AllButtons[indexWeek].BackColor = ButtonPressedColor;
            Solve(sender, e);
        }

        private void Clear(object sender, EventArgs e)
        {
            foreach (var kvp in AllButtons)
            {
                if (kvp.Value.Visible)
                {
                    kvp.Value.BackColor = ButtonDefaultColor;
                }
            }

            IsCleared = true;
        }
    }
}
