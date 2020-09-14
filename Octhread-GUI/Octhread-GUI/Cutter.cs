using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Octhread_GUI
{
    public partial class Cutter : Form
    {
        public string value = "";
        public Cutter(string name)
        {
            InitializeComponent();
            TextBoxCutterName.Text = name;

            CheckBoxNoCutFile.Checked = true;
            reset();
        }

        private void reset()
        {
            CheckBoxNoCutFile.Checked = true;
            TextBoxX.Text = "10.0";
            TextBoxY.Text = "10.0";
            TextBoxZ.Text = "10.0";

            TextBoxCutFileName.Enabled = false;
            TextBoxOverlap.Text = "0.001";
        }

        private void Cutter_Load(object sender, EventArgs e)
        {

        }
        public double toDouble(string text)
        {
            if (String.IsNullOrEmpty(text))
                return 0.0;
            return double.Parse(text, System.Globalization.NumberStyles.AllowDecimalPoint, System.Globalization.NumberFormatInfo.InvariantInfo);
        }
        private void buttonOK_Click(object sender, EventArgs e)
        {
            value = " --overlap " + TextBoxOverlap.Text;
            //Ok ça veut dire que le fichier est crée et qu'il est bon, et dans ce cas la, on l'utilise
            
            if (CheckBoxCutFile.Checked){
                value = " --cut " + TextBoxCutFileName.Text + " ";
            }
            else if (CheckBoxNoCutFile.Checked)
            {
                value += " --nocut " + toDouble(TextBoxX.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture) + " " + toDouble(TextBoxY.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture) + " " + toDouble(TextBoxZ.Text).ToString("0.000", System.Globalization.CultureInfo.InvariantCulture);
            }
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
        private void refuseLettersDouble(object sender, KeyPressEventArgs e)
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
        private void buttonCreation_Click(object sender, EventArgs e)
        {
            //Lorsqu'on click sur Creation, le programme de Baptiste doit s'ouvrir
            //Le text doit changer pour devenir .../NomFichier< /cut.info

            //string cmdPath = ".\\" + System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().CodeBase) + "\\";
            //string CmdArgs = TextBoxCutterName.Text;
            //string CmdCall = "datas\\CreateCutFile.exe";
            //string cmdText = cmdPath + CmdCall + CmdArgs;

            
        }

        private void buttonOpenCutFile_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "./";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFileDialog.FileName;
                    TextBoxCutFileName.Text = filePath;
                }
            }
        }

        private void buttonHelp_Click(object sender, EventArgs e)
        {
            string help = "On va chercher le fichier \"cutfile.txt\" ou quelque soit son nom, qui permettra de couper le fichier selon les axes X, Y et Z prédéfinies.\nNombre de découpe en X\nNombre de découpe en Y\nNombre de découpe en Z\nListe des coordonnées en X des découpes\nListe des coordonnées en Y des découpes\nListe des coordonnées en Z des découpes";
            help += "\n\nExemple :\n2\n3\n3\n-15 0\n-6 3 9\n6 12 18\n";

            MessageBox.Show(help);
        }

        private void buttonHelpOverlap_Click(object sender, EventArgs e)
        {
            string help = "Lors de la découpe, on peut rajouter de l'overlap. C'est à dire que 2 zones coupées côte à côte aurant une zone en commun de X metres, c'est cette valeur ici\n";

            MessageBox.Show(help);
        }
        private void ButtonHelpNoCutFile_Click(object sender, EventArgs e)
        {
            string help = "On va découpre le nuage de points en zones de tailles X,Y,Z à partir du point le plus bas en X,Y et Z\n";

            MessageBox.Show(help);
        }
        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            value = "";
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void CheckBoxNoCutFile_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckBoxNoCutFile.Checked)
            {
                TextBoxX.Enabled = true;
                TextBoxY.Enabled = true;
                TextBoxZ.Enabled = true;

                CheckBoxCutFile.Checked = false;
                TextBoxCutFileName.Enabled = false;
            }
        }

        private void CheckBoxCutFile_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckBoxCutFile.Checked)
            {
                CheckBoxNoCutFile.Checked = false;
                TextBoxX.Enabled = false;
                TextBoxY.Enabled = false;
                TextBoxZ.Enabled = false;
                
                TextBoxCutFileName.Enabled = true;

            }
        }

        private void TextBoxX_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxY_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxZ_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
