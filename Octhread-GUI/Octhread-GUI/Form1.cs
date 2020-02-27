﻿using System;
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

            /*Nombre div*/
            checkBoxNombreFichiers.Checked = false;
            textBoxNombreFichiers.Text = "";
            textBoxNombreFichiers.ReadOnly = true;
            textBoxNombreFichiers.BackColor = Color.PaleVioletRed;

            /*REMOVE*/
            checkBoxNombreFichiers.Enabled = false;
        }

        private void BackScreen_Load(object sender, EventArgs e)
        {
            ButtonReset_Click(sender, e);
        }

        private void GlobalLayout_Paint(object sender, PaintEventArgs e)
        {

        }

        private void readAndFillInfos(string path)
        {
            if (File.Exists(path))
            {
                string[] lines = System.IO.File.ReadAllLines(path);
                LabelFileName.Text = lines[0];
                LabelScanCount.Text = lines[1];
                int num = -1;
                if (Int32.TryParse(lines[1], out num))
                {
                    if (num == 1) // 1 unique scan
                    {
                        CheckBoxDistance.Enabled = false;
                    }
                    else
                    {
                        CheckBoxDistance.Enabled = true;
                    }
                }
                else
                {
                    CheckBoxDistance.Enabled = true;
                }
                long num_points = -1;
                if (Int64.TryParse(lines[2], out num_points))
                {
                    if (num_points == 0)
                    {
                        LabelPointCount.Text = "n/a";
                    }
                    else
                    {
                        LabelPointCount.Text = lines[2];
                    }
                }
                else
                {
                    LabelPointCount.Text = "n/a";
                }
            }
            else
                readAndFillInfosNA();
        }
        private void readAndFillInfosNA()
        {
            LabelFileName.Text = "n/a";
            LabelScanCount.Text = "n/a";
            LabelPointCount.Text = "n/a";
        }

        private void TextBoxSave_TextChanged(object sender, EventArgs e)
        {
            string path = TextBoxSave.Text;
            string ext = Path.GetExtension(path);
            
            if (ext.ToLower() == ".laz")
            {
                checkBoxPotree.Enabled = true;
            }
            else
            {
                checkBoxPotree.Enabled = false;
            }
        }

        private void ButtonOpenFile_TextChanged(object sender, EventArgs e)
        {

            string path = TextBoxOpen.Text; //absolute path of file (D:/Some Files/My file.ext)"
            string cmdCall = "datas\\GetInfo.exe"; //GetInfo is at the path datas/GetInfos.exe
            ProcessStartInfo startInfo = new ProcessStartInfo(); 

            startInfo.FileName = cmdCall;
            startInfo.Arguments = "\"" + path + "\""; //the problem seems to be here

            var proc = Process.Start(startInfo); //start the process 
            proc.WaitForExit(); //wait for it to finish
            
            string fileInfoName = Path.GetFileNameWithoutExtension(path) + ".info";
            if (proc.ExitCode == 0)
            {
                string name = "datas\\" + fileInfoName;
                readAndFillInfos(name);
            }
            else
            {
                readAndFillInfosNA();
            }

        }
        private void ButtonOpenFile_Click(object sender, EventArgs e)
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

        private void ButtonSaveFile_Click(object sender, EventArgs e)
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

        private void buttonHelpNombreFichiers_Click(object sender, EventArgs e)
        {
            string help = "Divisions.\n On va creer X fichier, X étant le nombre de divisions. Ils seront nommés [Nom]_[0-X]";
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
        private void checkBoxNombreFichiers_Click(object sender, EventArgs e)
        {
            if (checkBoxNombreFichiers.Checked)
            {
                textBoxNombreFichiers.Text = "4";
                textBoxNombreFichiers.ReadOnly = false;
                textBoxNombreFichiers.BackColor = Color.White;
            }
            else
            {
                textBoxNombreFichiers.Text = "";
                textBoxNombreFichiers.ReadOnly = true;
                textBoxNombreFichiers.BackColor = Color.PaleVioletRed;

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
                do_GammaCorrection = CheckBoxGammaCorrection.Checked,
                do_Div = checkBoxNombreFichiers.Checked,
                do_Potree = checkBoxPotree.Checked;

            double
                distMin = toDouble(TextBoxDistMin.Text),
                distMax = toDouble(TextBoxDistMax.Text),

                sizeDownSample = toDouble(TextBoxDownSampleSize.Text),

                SODevMultThresh = toDouble(TextBoxSODevMultThresh.Text);

            int
                SOMeanK = toInt(TextBoxSOMeanK.Text),
                TreeSize = toInt(TextBoxTreeSize.Text),
                division = toInt(textBoxNombreFichiers.Text);
                
            if (do_Tree)
            {
                //Ici, faire la vérification si l'arbre à déjà été créer ou non.
                string filename = Path.GetFileNameWithoutExtension(nameOpen);
                if (Directory.Exists(filename))
                {
                    DialogResult dialog = MessageBox.Show("Vous êtes sur le point de lancer la création de l'arbre, alors que le dossier \"" + filename + "\" existe déjà.\nC'est possible que l'arbre ai déjà été créer antérieurement.\nVoulez vous quand même continuer?", 
                        "Continue?", MessageBoxButtons.YesNo);
                    if (dialog == DialogResult.No)
                    {
                        return;
                    }
                }
            }
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
            if (do_Div)
            {
                CmdArgs += " --division " + division;
            }
            if (do_Potree)
            {
                CmdArgs += " --Potree true";
            }
          


            string cmdPath = ".\\" + System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().CodeBase) + "\\";

            string CmdCall = "datas\\nepascliquer.exe";
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
        

        private void buttonPrevisu_Click(object sender, EventArgs e)
        {
            //On ajoute Previsu
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
                do_GammaCorrection = CheckBoxGammaCorrection.Checked,
                do_Div = checkBoxNombreFichiers.Checked;


            double
                distMin = toDouble(TextBoxDistMin.Text),
                distMax = toDouble(TextBoxDistMax.Text),

                sizeDownSample = toDouble(TextBoxDownSampleSize.Text),

                SODevMultThresh = toDouble(TextBoxSODevMultThresh.Text);

            int
                SOMeanK = toInt(TextBoxSOMeanK.Text),
                TreeSize = toInt(TextBoxTreeSize.Text),
                division = toInt(textBoxNombreFichiers.Text);

            if (do_Tree)
            {
                //Ici, faire la vérification si l'arbre à déjà été créer ou non.
                string filename = Path.GetFileNameWithoutExtension(nameOpen);
                if (Directory.Exists(filename))
                {
                    DialogResult dialog = MessageBox.Show("Vous êtes sur le point de lancer la création de l'arbre, alors que le dossier \"" + filename + "\" existe déjà.\nC'est possible que l'arbre ai déjà été créer antérieurement.\nVoulez vous quand même continuer?",
                        "Continue?", MessageBoxButtons.YesNo);
                    if (dialog == DialogResult.No)
                    {
                        return;
                    }
                }
            }
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
            if (do_Div)
            {
                CmdArgs += " --division " + division;
            }

            CmdArgs += " --previs true";


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

        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tableLayoutPanel15_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }
    }
}
