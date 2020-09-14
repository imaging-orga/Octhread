namespace OutilsExterne_GUI
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.ComboBoxChoiceFunction = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.label2 = new System.Windows.Forms.Label();
            this.ButtonOpen = new System.Windows.Forms.Button();
            this.TextBoxOpen = new System.Windows.Forms.RichTextBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.ButtonSave = new System.Windows.Forms.Button();
            this.TextBoxSave = new System.Windows.Forms.RichTextBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.ButtonCancel = new System.Windows.Forms.Button();
            this.ButtonOK = new System.Windows.Forms.Button();
            this.TableLayoutCouperFichier = new System.Windows.Forms.TableLayoutPanel();
            this.CutLayoutPanel0 = new System.Windows.Forms.TableLayoutPanel();
            this.CutHelpCutFileButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.CutTextBoxCutFile = new System.Windows.Forms.RichTextBox();
            this.ButtonOpenCutFile = new System.Windows.Forms.Button();
            this.CutCheckBoxCutFile = new System.Windows.Forms.CheckBox();
            this.CutLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.CutHelpNoCutFileButton = new System.Windows.Forms.Button();
            this.labelX = new System.Windows.Forms.Label();
            this.labelY = new System.Windows.Forms.Label();
            this.labelZ = new System.Windows.Forms.Label();
            this.CutTextBoxX = new System.Windows.Forms.RichTextBox();
            this.CutTextBoxY = new System.Windows.Forms.RichTextBox();
            this.CutTextBoxZ = new System.Windows.Forms.RichTextBox();
            this.CutCheckBoxNoCutFile = new System.Windows.Forms.CheckBox();
            this.CutLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.CutHelpOverlapButton = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.CutTextBoxOverlap = new System.Windows.Forms.RichTextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.TableLayoutCouperFichier.SuspendLayout();
            this.CutLayoutPanel0.SuspendLayout();
            this.CutLayoutPanel1.SuspendLayout();
            this.CutLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.ComboBoxChoiceFunction, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel4, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.TableLayoutCouperFichier, 0, 3);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(716, 506);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // ComboBoxChoiceFunction
            // 
            this.ComboBoxChoiceFunction.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.ComboBoxChoiceFunction.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboBoxChoiceFunction.FormattingEnabled = true;
            this.ComboBoxChoiceFunction.Items.AddRange(new object[] {
            "Couper Fichier",
            "Fichier Sans Points",
            "Optimize"});
            this.ComboBoxChoiceFunction.Location = new System.Drawing.Point(3, 111);
            this.ComboBoxChoiceFunction.Name = "ComboBoxChoiceFunction";
            this.ComboBoxChoiceFunction.Size = new System.Drawing.Size(710, 28);
            this.ComboBoxChoiceFunction.Sorted = true;
            this.ComboBoxChoiceFunction.TabIndex = 0;
            this.ComboBoxChoiceFunction.SelectedIndexChanged += new System.EventHandler(this.ComboBoxChoiceFunction_SelectedIndexChanged);
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.ButtonOpen, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.TextBoxOpen, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(710, 44);
            this.tableLayoutPanel2.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(144, 20);
            this.label2.TabIndex = 1;
            this.label2.Text = "Ouvrir";
            // 
            // ButtonOpen
            // 
            this.ButtonOpen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ButtonOpen.Location = new System.Drawing.Point(153, 3);
            this.ButtonOpen.Name = "ButtonOpen";
            this.ButtonOpen.Size = new System.Drawing.Size(44, 38);
            this.ButtonOpen.TabIndex = 2;
            this.ButtonOpen.Text = "...";
            this.ButtonOpen.UseVisualStyleBackColor = true;
            this.ButtonOpen.Click += new System.EventHandler(this.ButtonOpen_Click);
            // 
            // TextBoxOpen
            // 
            this.TextBoxOpen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TextBoxOpen.Location = new System.Drawing.Point(203, 3);
            this.TextBoxOpen.Name = "TextBoxOpen";
            this.TextBoxOpen.Size = new System.Drawing.Size(504, 38);
            this.TextBoxOpen.TabIndex = 3;
            this.TextBoxOpen.Text = "";
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.ButtonSave, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.TextBoxSave, 2, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 53);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(710, 44);
            this.tableLayoutPanel3.TabIndex = 2;
            this.tableLayoutPanel3.Paint += new System.Windows.Forms.PaintEventHandler(this.tableLayoutPanel3_Paint);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(144, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Sauvegarde";
            // 
            // ButtonSave
            // 
            this.ButtonSave.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ButtonSave.Location = new System.Drawing.Point(153, 3);
            this.ButtonSave.Name = "ButtonSave";
            this.ButtonSave.Size = new System.Drawing.Size(44, 38);
            this.ButtonSave.TabIndex = 1;
            this.ButtonSave.Text = "...";
            this.ButtonSave.UseVisualStyleBackColor = true;
            this.ButtonSave.Click += new System.EventHandler(this.ButtonSave_Click);
            // 
            // TextBoxSave
            // 
            this.TextBoxSave.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TextBoxSave.Location = new System.Drawing.Point(203, 3);
            this.TextBoxSave.Name = "TextBoxSave";
            this.TextBoxSave.Size = new System.Drawing.Size(504, 38);
            this.TextBoxSave.TabIndex = 2;
            this.TextBoxSave.Text = "";
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel4.ColumnCount = 3;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanel4.Controls.Add(this.ButtonCancel, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.ButtonOK, 2, 0);
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 459);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(710, 44);
            this.tableLayoutPanel4.TabIndex = 3;
            // 
            // ButtonCancel
            // 
            this.ButtonCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ButtonCancel.Location = new System.Drawing.Point(413, 3);
            this.ButtonCancel.Name = "ButtonCancel";
            this.ButtonCancel.Size = new System.Drawing.Size(144, 38);
            this.ButtonCancel.TabIndex = 1;
            this.ButtonCancel.Text = "Annuler";
            this.ButtonCancel.UseVisualStyleBackColor = true;
            this.ButtonCancel.Click += new System.EventHandler(this.ButtonCancel_Click);
            // 
            // ButtonOK
            // 
            this.ButtonOK.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ButtonOK.Location = new System.Drawing.Point(563, 3);
            this.ButtonOK.Name = "ButtonOK";
            this.ButtonOK.Size = new System.Drawing.Size(144, 38);
            this.ButtonOK.TabIndex = 2;
            this.ButtonOK.Text = "OK";
            this.ButtonOK.UseVisualStyleBackColor = true;
            this.ButtonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // TableLayoutCouperFichier
            // 
            this.TableLayoutCouperFichier.ColumnCount = 1;
            this.TableLayoutCouperFichier.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutCouperFichier.Controls.Add(this.CutLayoutPanel0, 0, 0);
            this.TableLayoutCouperFichier.Controls.Add(this.CutLayoutPanel1, 0, 1);
            this.TableLayoutCouperFichier.Controls.Add(this.CutLayoutPanel2, 0, 3);
            this.TableLayoutCouperFichier.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TableLayoutCouperFichier.Location = new System.Drawing.Point(3, 153);
            this.TableLayoutCouperFichier.Name = "TableLayoutCouperFichier";
            this.TableLayoutCouperFichier.RowCount = 5;
            this.TableLayoutCouperFichier.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.TableLayoutCouperFichier.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.TableLayoutCouperFichier.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.TableLayoutCouperFichier.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.TableLayoutCouperFichier.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutCouperFichier.Size = new System.Drawing.Size(710, 300);
            this.TableLayoutCouperFichier.TabIndex = 4;
            this.TableLayoutCouperFichier.Visible = false;
            // 
            // CutLayoutPanel0
            // 
            this.CutLayoutPanel0.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.CutLayoutPanel0.ColumnCount = 6;
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 300F));
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel0.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel0.Controls.Add(this.CutHelpCutFileButton, 0, 0);
            this.CutLayoutPanel0.Controls.Add(this.label3, 2, 0);
            this.CutLayoutPanel0.Controls.Add(this.CutTextBoxCutFile, 3, 0);
            this.CutLayoutPanel0.Controls.Add(this.ButtonOpenCutFile, 4, 0);
            this.CutLayoutPanel0.Controls.Add(this.CutCheckBoxCutFile, 5, 0);
            this.CutLayoutPanel0.Location = new System.Drawing.Point(3, 3);
            this.CutLayoutPanel0.Name = "CutLayoutPanel0";
            this.CutLayoutPanel0.RowCount = 1;
            this.CutLayoutPanel0.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel0.Size = new System.Drawing.Size(704, 44);
            this.CutLayoutPanel0.TabIndex = 0;
            // 
            // CutHelpCutFileButton
            // 
            this.CutHelpCutFileButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutHelpCutFileButton.Location = new System.Drawing.Point(3, 3);
            this.CutHelpCutFileButton.Name = "CutHelpCutFileButton";
            this.CutHelpCutFileButton.Size = new System.Drawing.Size(44, 38);
            this.CutHelpCutFileButton.TabIndex = 0;
            this.CutHelpCutFileButton.Text = "?";
            this.CutHelpCutFileButton.UseVisualStyleBackColor = true;
            this.CutHelpCutFileButton.Click += new System.EventHandler(this.CutHelpCutFileButton_Click);
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(157, 12);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(144, 20);
            this.label3.TabIndex = 1;
            this.label3.Text = "Cut File";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // CutTextBoxCutFile
            // 
            this.CutTextBoxCutFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutTextBoxCutFile.Location = new System.Drawing.Point(307, 3);
            this.CutTextBoxCutFile.Name = "CutTextBoxCutFile";
            this.CutTextBoxCutFile.Size = new System.Drawing.Size(294, 38);
            this.CutTextBoxCutFile.TabIndex = 2;
            this.CutTextBoxCutFile.Text = "";
            // 
            // ButtonOpenCutFile
            // 
            this.ButtonOpenCutFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ButtonOpenCutFile.Location = new System.Drawing.Point(607, 3);
            this.ButtonOpenCutFile.Name = "ButtonOpenCutFile";
            this.ButtonOpenCutFile.Size = new System.Drawing.Size(44, 38);
            this.ButtonOpenCutFile.TabIndex = 3;
            this.ButtonOpenCutFile.Text = "...";
            this.ButtonOpenCutFile.UseVisualStyleBackColor = true;
            this.ButtonOpenCutFile.Click += new System.EventHandler(this.ButtonOpenCutFile_Click);
            // 
            // CutCheckBoxCutFile
            // 
            this.CutCheckBoxCutFile.AutoSize = true;
            this.CutCheckBoxCutFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutCheckBoxCutFile.Location = new System.Drawing.Point(657, 3);
            this.CutCheckBoxCutFile.Name = "CutCheckBoxCutFile";
            this.CutCheckBoxCutFile.Size = new System.Drawing.Size(44, 38);
            this.CutCheckBoxCutFile.TabIndex = 4;
            this.CutCheckBoxCutFile.UseVisualStyleBackColor = true;
            this.CutCheckBoxCutFile.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // CutLayoutPanel1
            // 
            this.CutLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.CutLayoutPanel1.ColumnCount = 9;
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.CutLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel1.Controls.Add(this.CutHelpNoCutFileButton, 0, 0);
            this.CutLayoutPanel1.Controls.Add(this.labelX, 2, 0);
            this.CutLayoutPanel1.Controls.Add(this.labelY, 4, 0);
            this.CutLayoutPanel1.Controls.Add(this.labelZ, 6, 0);
            this.CutLayoutPanel1.Controls.Add(this.CutTextBoxX, 3, 0);
            this.CutLayoutPanel1.Controls.Add(this.CutTextBoxY, 5, 0);
            this.CutLayoutPanel1.Controls.Add(this.CutTextBoxZ, 7, 0);
            this.CutLayoutPanel1.Controls.Add(this.CutCheckBoxNoCutFile, 8, 0);
            this.CutLayoutPanel1.Location = new System.Drawing.Point(3, 53);
            this.CutLayoutPanel1.Name = "CutLayoutPanel1";
            this.CutLayoutPanel1.RowCount = 1;
            this.CutLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel1.Size = new System.Drawing.Size(704, 44);
            this.CutLayoutPanel1.TabIndex = 1;
            // 
            // CutHelpNoCutFileButton
            // 
            this.CutHelpNoCutFileButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutHelpNoCutFileButton.Location = new System.Drawing.Point(3, 3);
            this.CutHelpNoCutFileButton.Name = "CutHelpNoCutFileButton";
            this.CutHelpNoCutFileButton.Size = new System.Drawing.Size(44, 38);
            this.CutHelpNoCutFileButton.TabIndex = 0;
            this.CutHelpNoCutFileButton.Text = "?";
            this.CutHelpNoCutFileButton.UseVisualStyleBackColor = true;
            this.CutHelpNoCutFileButton.Click += new System.EventHandler(this.CutHelpNoCutFileButton_Click);
            // 
            // labelX
            // 
            this.labelX.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelX.AutoSize = true;
            this.labelX.Location = new System.Drawing.Point(57, 12);
            this.labelX.Name = "labelX";
            this.labelX.Size = new System.Drawing.Size(44, 20);
            this.labelX.TabIndex = 1;
            this.labelX.Text = "X";
            this.labelX.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelY
            // 
            this.labelY.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelY.AutoSize = true;
            this.labelY.Location = new System.Drawing.Point(257, 12);
            this.labelY.Name = "labelY";
            this.labelY.Size = new System.Drawing.Size(44, 20);
            this.labelY.TabIndex = 2;
            this.labelY.Text = "Y";
            this.labelY.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelZ
            // 
            this.labelZ.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelZ.AutoSize = true;
            this.labelZ.Location = new System.Drawing.Point(457, 12);
            this.labelZ.Name = "labelZ";
            this.labelZ.Size = new System.Drawing.Size(44, 20);
            this.labelZ.TabIndex = 3;
            this.labelZ.Text = "Z";
            this.labelZ.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // CutTextBoxX
            // 
            this.CutTextBoxX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutTextBoxX.Location = new System.Drawing.Point(107, 3);
            this.CutTextBoxX.Name = "CutTextBoxX";
            this.CutTextBoxX.Size = new System.Drawing.Size(144, 38);
            this.CutTextBoxX.TabIndex = 4;
            this.CutTextBoxX.Text = "10.0";
            this.CutTextBoxX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.refuseLettersDouble);
            // 
            // CutTextBoxY
            // 
            this.CutTextBoxY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutTextBoxY.Location = new System.Drawing.Point(307, 3);
            this.CutTextBoxY.Name = "CutTextBoxY";
            this.CutTextBoxY.Size = new System.Drawing.Size(144, 38);
            this.CutTextBoxY.TabIndex = 5;
            this.CutTextBoxY.Text = "10.0";
            this.CutTextBoxY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.refuseLettersDouble);
            // 
            // CutTextBoxZ
            // 
            this.CutTextBoxZ.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutTextBoxZ.Location = new System.Drawing.Point(507, 3);
            this.CutTextBoxZ.Name = "CutTextBoxZ";
            this.CutTextBoxZ.Size = new System.Drawing.Size(144, 38);
            this.CutTextBoxZ.TabIndex = 6;
            this.CutTextBoxZ.Text = "10.0";
            this.CutTextBoxZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.refuseLettersDouble);
            // 
            // CutCheckBoxNoCutFile
            // 
            this.CutCheckBoxNoCutFile.AutoSize = true;
            this.CutCheckBoxNoCutFile.Checked = true;
            this.CutCheckBoxNoCutFile.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CutCheckBoxNoCutFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutCheckBoxNoCutFile.Location = new System.Drawing.Point(657, 3);
            this.CutCheckBoxNoCutFile.Name = "CutCheckBoxNoCutFile";
            this.CutCheckBoxNoCutFile.Size = new System.Drawing.Size(44, 38);
            this.CutCheckBoxNoCutFile.TabIndex = 7;
            this.CutCheckBoxNoCutFile.UseVisualStyleBackColor = true;
            this.CutCheckBoxNoCutFile.CheckedChanged += new System.EventHandler(this.CutCheckBoxNoCutFile_CheckedChanged);
            // 
            // CutLayoutPanel2
            // 
            this.CutLayoutPanel2.ColumnCount = 5;
            this.CutLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.CutLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 300F));
            this.CutLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.CutLayoutPanel2.Controls.Add(this.CutHelpOverlapButton, 0, 0);
            this.CutLayoutPanel2.Controls.Add(this.label4, 2, 0);
            this.CutLayoutPanel2.Controls.Add(this.CutTextBoxOverlap, 3, 0);
            this.CutLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutLayoutPanel2.Location = new System.Drawing.Point(3, 133);
            this.CutLayoutPanel2.Name = "CutLayoutPanel2";
            this.CutLayoutPanel2.RowCount = 1;
            this.CutLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.CutLayoutPanel2.Size = new System.Drawing.Size(704, 44);
            this.CutLayoutPanel2.TabIndex = 2;
            // 
            // CutHelpOverlapButton
            // 
            this.CutHelpOverlapButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutHelpOverlapButton.Location = new System.Drawing.Point(3, 3);
            this.CutHelpOverlapButton.Name = "CutHelpOverlapButton";
            this.CutHelpOverlapButton.Size = new System.Drawing.Size(44, 38);
            this.CutHelpOverlapButton.TabIndex = 0;
            this.CutHelpOverlapButton.Text = "?";
            this.CutHelpOverlapButton.UseVisualStyleBackColor = true;
            this.CutHelpOverlapButton.Click += new System.EventHandler(this.CutHelpOverlapButton_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Location = new System.Drawing.Point(207, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(144, 44);
            this.label4.TabIndex = 1;
            this.label4.Text = "Overlap(m)";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // CutTextBoxOverlap
            // 
            this.CutTextBoxOverlap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CutTextBoxOverlap.Location = new System.Drawing.Point(357, 3);
            this.CutTextBoxOverlap.Name = "CutTextBoxOverlap";
            this.CutTextBoxOverlap.Size = new System.Drawing.Size(294, 38);
            this.CutTextBoxOverlap.TabIndex = 2;
            this.CutTextBoxOverlap.Text = "0.001";
            this.CutTextBoxOverlap.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.refuseLettersDouble);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(720, 310);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 26);
            this.textBox1.TabIndex = 1;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(716, 506);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "Form1";
            this.Text = "OutilsExterne-GUI";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.TableLayoutCouperFichier.ResumeLayout(false);
            this.CutLayoutPanel0.ResumeLayout(false);
            this.CutLayoutPanel0.PerformLayout();
            this.CutLayoutPanel1.ResumeLayout(false);
            this.CutLayoutPanel1.PerformLayout();
            this.CutLayoutPanel2.ResumeLayout(false);
            this.CutLayoutPanel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ComboBox ComboBoxChoiceFunction;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Button ButtonCancel;
        private System.Windows.Forms.TableLayoutPanel TableLayoutCouperFichier;
        private System.Windows.Forms.TableLayoutPanel CutLayoutPanel0;
        private System.Windows.Forms.Button CutHelpCutFileButton;
        private System.Windows.Forms.TableLayoutPanel CutLayoutPanel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox CutTextBoxCutFile;
        private System.Windows.Forms.Button ButtonOpenCutFile;
        private System.Windows.Forms.CheckBox CutCheckBoxCutFile;
        private System.Windows.Forms.Button CutHelpNoCutFileButton;
        private System.Windows.Forms.Label labelX;
        private System.Windows.Forms.Label labelY;
        private System.Windows.Forms.Label labelZ;
        private System.Windows.Forms.RichTextBox CutTextBoxX;
        private System.Windows.Forms.RichTextBox CutTextBoxY;
        private System.Windows.Forms.RichTextBox CutTextBoxZ;
        private System.Windows.Forms.CheckBox CutCheckBoxNoCutFile;
        private System.Windows.Forms.Button ButtonOpen;
        private System.Windows.Forms.RichTextBox TextBoxOpen;
        private System.Windows.Forms.Button ButtonSave;
        private System.Windows.Forms.RichTextBox TextBoxSave;
        private System.Windows.Forms.TableLayoutPanel CutLayoutPanel2;
        private System.Windows.Forms.Button CutHelpOverlapButton;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RichTextBox CutTextBoxOverlap;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button ButtonOK;
    }
}

