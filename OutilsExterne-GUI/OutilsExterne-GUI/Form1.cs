using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
namespace OutilsExterne_GUI
{

    public partial class Form1 : Form
    {

        private bool getArgs = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void refuseLettersDouble(object sender, KeyPressEventArgs e)
        {
            // Verify that the pressed key isn't CTRL or any non-numeric digit
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar) && (e.KeyChar != '.'))
            {
                e.Handled = true;
            }
            // If you want, you can allow decimal (float) numbers
            if ((e.KeyChar == '.') && ((sender as RichTextBox).Text.IndexOf('.') > -1))
            {
                e.Handled = true;
            }
        }
        private void refuseLettersDoubleTextBox(object sender, KeyPressEventArgs e)
        {
            // Verify that the pressed key isn't CTRL or any non-numeric digit
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar) && (e.KeyChar != '.'))
            {
                e.Handled = true;
            }
            // If you want, you can allow decimal (float) numbers
            if ((e.KeyChar == '.') && ((sender as TextBox).Text.IndexOf('.') > -1))
            {
                e.Handled = true;
            }
        }
        private void refuseLettersInt(object sender, KeyPressEventArgs e)
        {
            // Verify that the pressed key isn't CTRL or any non-numeric digit
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void couperFichier()
        {

        }
        private void ComboBoxChoiceFunction_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (getArgs)
            {
                this.tableLayoutPanel1.Controls.RemoveAt(4);
            }
            switch (ComboBoxChoiceFunction.SelectedIndex) {

                case 0:
                    getArgs = true;
                    this.tableLayoutPanel1.Controls.Add(TableLayoutCouperFichier, 0, 3);
                    this.TableLayoutCouperFichier.Visible = true;
                    break;

                case 1:
                    getArgs = false;
                    break;

                case 2:
                    getArgs = false;
                    break;
                default:
                    break;
            }//Couper fichier
        }

        private void tableLayoutPanel3_Paint(object sender, PaintEventArgs e)
        {

        }



        private void CutHelpCutFileButton_Click(object sender, EventArgs e)
        {
            string help = "On va chercher le fichier \"cutfile.txt\" ou quelque soit son nom, qui permettra de couper le fichier selon les axes X, Y et Z prédéfinies.\nNombre de découpe en X\nNombre de découpe en Y\nNombre de découpe en Z\nListe des coordonnées en X des découpes\nListe des coordonnées en Y des découpes\nListe des coordonnées en Z des découpes";
            help += "\n\nExemple :\n2\n3\n3\n-15 0\n-6 3 9\n6 12 18\n";

            MessageBox.Show(help);
        }

        private void CutHelpNoCutFileButton_Click(object sender, EventArgs e)
        {
            string help = "On va découpre le nuage de points en zones de tailles X,Y,Z à partir du point le plus bas en X,Y et Z\n";

            MessageBox.Show(help);
        }

        private void CutHelpOverlapButton_Click(object sender, EventArgs e)
        {
            string help = "Lors de la découpe, on peut rajouter de l'overlap. C'est à dire que 2 zones coupées côte à côte aurant une zone en commun de X metres, c'est cette valeur ici\n";

            MessageBox.Show(help);
        }

        private void CutCheckBoxNoCutFile_CheckedChanged(object sender, EventArgs e)
        {
            if (CutCheckBoxNoCutFile.Checked)
            {
                CutCheckBoxCutFile.Checked = false;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (CutCheckBoxCutFile.Checked)
            {
                CutCheckBoxNoCutFile.Checked = false;
            }
        }
        private double toDouble(string text)
        {
            if (String.IsNullOrEmpty(text))
                return 0.0;
            return double.Parse(text, System.Globalization.NumberStyles.AllowDecimalPoint, System.Globalization.NumberFormatInfo.InvariantInfo);
        }
        private void buttonOK_Click(object sender, EventArgs e)
        {
            string nameOpen = TextBoxOpen.Text;
            string nameSave = TextBoxSave.Text;
            if (nameOpen.Equals("") || nameSave.Equals(""))
            {
                MessageBox.Show("Fichiers Non spécifiés");
                return;
            }
            string CmdArgs = " --nameIn \"" + nameOpen + "\" --nameOut \"" + nameSave + "\"";
            switch (ComboBoxChoiceFunction.SelectedIndex)
            {
                case 0:
                    if (CutCheckBoxNoCutFile.Checked)
                    {
                        CmdArgs += " --nocut " + toDouble(CutTextBoxX.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture) + " " + toDouble(CutTextBoxY.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture) + " " + toDouble(CutTextBoxZ.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (CutCheckBoxCutFile.Checked)
                    {
                        CmdArgs += " --cut " + CutTextBoxCutFile.Text + " ";
                    }
                    CmdArgs += " --overlap " + CutTextBoxOverlap.Text;
                    break;
                case 1:
                    CmdArgs += " --fileWithoutPoints true";
                    break;
                case 2:
                    CmdArgs += " --optimize true";
                    break;
                default:
                    break;
            }

            string[] lines = new string[1];
            lines[0] = CmdArgs;
            System.IO.File.AppendAllLines(nameSave + ".txt", lines);


            string cmdPath = ".\\" + System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().CodeBase) + "\\";
            string CmdCall = "OutilsExternes.exe";
            string cmdText = cmdPath + CmdCall + CmdArgs;
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = CmdCall;
            startInfo.Arguments = CmdArgs;
            //MessageBox.Show(cmdText);
            Process.Start(startInfo);
        }

        private void ButtonOpen_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "./";
                openFileDialog.Filter = "All Files(*.pts, *.e57, *.las, *.laz)|*.pts;*.e57;*.laz;*.las;|pts files(*.pts)|*.pts|e57 files(*.e57)|*.e57|las files (*.las, *.laz)|*.las; *.laz";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFileDialog.FileName;
                    TextBoxOpen.Text = filePath;
                }
            }
        }

        private void ButtonSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "All Files(*.pts, *.e57, *.las, *.laz)|*.pts;*.e57;*.laz;*.las |pts files(*.pts)|*.pts|e57 files(*.e57)|*.e57|las files (*.las, *.laz)|*.las; *.laz";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                TextBoxSave.Text = saveFileDialog1.FileName;
            }
        }

        private void ButtonOpenCutFile_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "./";
                openFileDialog.Filter = "All Files|*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFileDialog.FileName;
                    CutTextBoxCutFile.Text = filePath;
                }
            }
        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.Application.ExitThread();
        }
    }
}
