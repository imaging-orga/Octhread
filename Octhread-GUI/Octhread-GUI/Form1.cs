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

namespace Octhread_GUI
{
    public partial class BackScreen : Form
    {
        public BackScreen()
        {
            InitializeComponent();
        }

        private void ButtonReset_Click(object sender, EventArgs e)
        {
            /*Tree*/
            CheckBoxTree.Checked = true;
            TextBoxTreeSize.ReadOnly = false;
            TextBoxTreeSize.Text = "134217728";
            /*Distance*/
            CheckBoxDistance.Checked = false;

            TextBoxDistMin.ReadOnly = true;
            TextBoxDistMin.Text = "";
            TextBoxDistMin.BackColor = Color.PaleVioletRed;

            TextBoxDistMax.ReadOnly = true;
            TextBoxDistMax.Text = "";
            TextBoxDistMax.BackColor = Color.PaleVioletRed;

            /*Sous Echantillonnage*/
            CheckBoxDownSample.Checked = false;

            TextBoxDownSampleSize.ReadOnly = true;
            TextBoxDownSampleSize.Text = "";
            TextBoxDownSampleSize.BackColor = Color.PaleVioletRed;

            /*Points Isolés*/
            CheckBoxRemoveOutliers.Checked = false;

            TextBoxSOMeanK.ReadOnly = true;
            TextBoxSOMeanK.Text = "";
            TextBoxSOMeanK.BackColor = Color.PaleVioletRed;

            TextBoxSODevMultThresh.ReadOnly = true;
            TextBoxSODevMultThresh.Text = "";
            TextBoxSODevMultThresh.BackColor = Color.PaleVioletRed;

            /*Correction Gamma*/
            CheckBoxGammaCorrection.Checked = true;
        }

        private void BackScreen_Load(object sender, EventArgs e)
        {
            ButtonReset_Click(sender, e);
        }

        private void GlobalLayout_Paint(object sender, PaintEventArgs e)
        {

        }

        private void ButtonOpenFile_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "./";
                openFileDialog.Filter = "ptx files|*.ptx|e57 files|*.e57";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    string filePath = openFileDialog.FileName;
                    TextBoxOpen.Text = filePath;
                }
            }
        }

        private void ButtonSaveFile_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "ptx files|*.ptx|e57 files|*.e57";
            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                TextBoxSave.Text = saveFileDialog1.FileName;
            }
        }

        private void ButtonHelpTree_Click(object sender, EventArgs e)
        {
            string help = "Est-ce que l'arbre à déjà été construit? Si oui, décochez cette case. Sinon, cochez la.\n[nombre de points par fichier] correspond au nombre max de points qui sera contenu par fichier. Par défaut il est a 1024*1024*128 (environ des fichiers de 4Go)";
            MessageBox.Show(help);
        }

        private void ButtonHelpDistance_Click(object sender, EventArgs e)
        {
            string help = "Fonction de distance.\n La fonction va enlever tous les points qui ne sont pas compris entre [Minimum] et [Maximum] mètres du centre du scan.";
            MessageBox.Show(help);
        }

        private void ButtonHelpDownSample_Click(object sender, EventArgs e)
        {
            string help = "Fonction de sous-échantillonnage.\n La fonction va sous-échantillonner le nuage de points à [Taille] mètres.";
            MessageBox.Show(help);
        }

        private void ButtonHelpRemoveOutliers_Click(object sender, EventArgs e)
        {
            string help = "Fonction de points isolés.\n La fonction va calculer la distance moyenne d'un point avec ses [Nombre voisins] voisins les plus proche,\n" +
                "puis va rejeter les points qui ont une distance à ses voisins les plus proche plus grand que la distance moyenne plus [Multiplicateur] fois la déviation standard";
            MessageBox.Show(help);
        }

        private void ButtonHelpCorrectionGamma_Click(object sender, EventArgs e)
        {
            string help = "Correction Gamma.\n La fonction va permettre de rendre les points pour les scans qui ne sont pas en couleurs un peu plus lumineux.";
            MessageBox.Show(help);
        }

        private void CheckBoxTree_Click(object sender, EventArgs e)
        {
            if (CheckBoxTree.Checked)
            {
                TextBoxTreeSize.Text = "134217728";
                TextBoxTreeSize.ReadOnly = false;
                TextBoxTreeSize.BackColor = Color.White;
            }
            else
            {
                TextBoxTreeSize.Text = "";
                TextBoxTreeSize.ReadOnly = true;
                TextBoxTreeSize.BackColor = Color.PaleVioletRed;
            }
        }

        private void CheckBoxDistance_Click(object sender, EventArgs e)
        {
            if (CheckBoxDistance.Checked)
            {
                TextBoxDistMin.Text = "0.0";
                TextBoxDistMax.Text = "15.0";
                TextBoxDistMin.ReadOnly = false;
                TextBoxDistMax.ReadOnly = false;
                TextBoxDistMin.BackColor = Color.White;
                TextBoxDistMax.BackColor = Color.White;
            }
            else
            {
                TextBoxDistMin.Text = "";
                TextBoxDistMax.Text = "";
                TextBoxDistMin.ReadOnly = true;
                TextBoxDistMax.ReadOnly = true;
                TextBoxDistMin.BackColor = Color.PaleVioletRed;
                TextBoxDistMax.BackColor = Color.PaleVioletRed;
            }
        }

        private void CheckBoxDownSample_Click(object sender, EventArgs e)
        {
            if (CheckBoxDownSample.Checked)
            {
                TextBoxDownSampleSize.Text = "0.005";
                TextBoxDownSampleSize.ReadOnly = false;
                TextBoxDownSampleSize.BackColor = Color.White;
            }
            else
            {
                TextBoxDownSampleSize.Text = "";
                TextBoxDownSampleSize.ReadOnly = true;
                TextBoxDownSampleSize.BackColor = Color.PaleVioletRed;

            }
        }

        private void CheckBoxRemoveOutliers_Click(object sender, EventArgs e)
        {
            if (CheckBoxRemoveOutliers.Checked)
            {
                TextBoxSODevMultThresh.Text = "1";
                TextBoxSOMeanK.Text = "20";
                TextBoxSODevMultThresh.ReadOnly = false;
                TextBoxSOMeanK.ReadOnly = false;
                TextBoxSODevMultThresh.BackColor = Color.White;
                TextBoxSOMeanK.BackColor = Color.White;

            }
            else
            {
                TextBoxSODevMultThresh.Text = "";
                TextBoxSOMeanK.Text = "";
                TextBoxSODevMultThresh.ReadOnly = true;
                TextBoxSOMeanK.ReadOnly = true;
                TextBoxSODevMultThresh.BackColor = Color.PaleVioletRed;
                TextBoxSOMeanK.BackColor = Color.PaleVioletRed;
            }
        }

        private void CheckBoxGammaCorrection_Click(object sender, EventArgs e)
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

        private void refuseLettersInt(object sender, KeyPressEventArgs e)
        {
            // Verify that the pressed key isn't CTRL or any non-numeric digit
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }
        private void TextBoxDistMin_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxDistMax_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxDownSampleSize_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxSOMeanK_TextChanged(object sender, EventArgs e)
        {

        }

        private void TextBoxSODevMultThresh_TextChanged(object sender, EventArgs e)
        {

        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            Dispose(true);
        }



        public double toDouble(string text)
        {
            if (String.IsNullOrEmpty(text))
                return 0.0;
            return double.Parse(text, System.Globalization.NumberStyles.AllowDecimalPoint, System.Globalization.NumberFormatInfo.InvariantInfo);
        }

        public int toInt(string text)
        {
            if (String.IsNullOrEmpty(text))
                return 0;
            return int.Parse(text);
        }
        private void ButtonApply_Click(object sender, EventArgs e)
        {
            string nameOpen = TextBoxOpen.Text;
            string nameSave = TextBoxSave.Text;
            if (nameOpen.Equals("") || nameSave.Equals(""))
            {
                MessageBox.Show("Fichiers Non spécifiés");
                return;
            }




            bool
                do_Tree = CheckBoxTree.Checked,
                do_Distance = CheckBoxDistance.Checked,
                do_DownSample = CheckBoxDownSample.Checked,
                do_RemoveOutliers = CheckBoxRemoveOutliers.Checked,
                do_GammaCorrection = CheckBoxGammaCorrection.Checked;

            double
                distMin = toDouble(TextBoxDistMin.Text),
                distMax = toDouble(TextBoxDistMax.Text),

                sizeDownSample = toDouble(TextBoxDownSampleSize.Text),

                SODevMultThresh = toDouble(TextBoxSODevMultThresh.Text);

            int
                SOMeanK = toInt(TextBoxSOMeanK.Text),
                TreeSize = toInt(TextBoxTreeSize.Text);



            string CmdArgs = " --nameIn \"" + nameOpen + "\" --nameOut \"" + nameSave + "\"";
            if (do_Tree)
            {
                CmdArgs += " --createTree " + TreeSize;
            }
            if (do_Distance)
            {
                CmdArgs += " --distance " + distMin.ToString("0.000", System.Globalization.CultureInfo.InvariantCulture) + " " + distMax.ToString("0.000", System.Globalization.CultureInfo.InvariantCulture);
            }
            if (do_DownSample)
            {
                CmdArgs += " --downSample " + sizeDownSample.ToString("0.000", System.Globalization.CultureInfo.InvariantCulture);
            }
            if (do_RemoveOutliers)
            {
                CmdArgs += " --removeOutliers " + SOMeanK + " " + SODevMultThresh.ToString("0.000", System.Globalization.CultureInfo.InvariantCulture);
            }
            if (do_GammaCorrection)
            {
                CmdArgs += " --correctionGamma true";
            }


            string cmdPath = ".\\" + System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().CodeBase) + "\\";

            string CmdCall = "nepascliquer.exe";
            string cmdText = cmdPath + CmdCall + CmdArgs;
            ProcessStartInfo startInfo = new ProcessStartInfo();
            TextBoxWrite.Text = CmdArgs;
            startInfo.FileName = CmdCall;
            startInfo.Arguments = CmdArgs;
            //MessageBox.Show(cmdText);
            Process.Start(startInfo);
        }

        private void CheckBoxTree_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
