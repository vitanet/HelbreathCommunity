<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Launcher
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Launcher))
        Me.strtBtn = New System.Windows.Forms.Button()
        Me.progress = New System.Windows.Forms.ProgressBar()
        Me.backgroundWorker1 = New System.ComponentModel.BackgroundWorker()
        Me.BackgroundWorker2 = New System.ComponentModel.BackgroundWorker()
        Me.showinfo = New System.Windows.Forms.TextBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.closeBtn = New System.Windows.Forms.Button()
        Me.minimizeBtn = New System.Windows.Forms.Button()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'strtBtn
        '
        Me.strtBtn.BackColor = System.Drawing.Color.FromArgb(CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer))
        Me.strtBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(121, Byte), Integer), CType(CType(203, Byte), Integer))
        Me.strtBtn.FlatAppearance.BorderSize = 2
        Me.strtBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.strtBtn.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.strtBtn.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(121, Byte), Integer), CType(CType(203, Byte), Integer))
        Me.strtBtn.Location = New System.Drawing.Point(754, 396)
        Me.strtBtn.Name = "strtBtn"
        Me.strtBtn.Size = New System.Drawing.Size(150, 62)
        Me.strtBtn.TabIndex = 18
        Me.strtBtn.Text = "START GAME"
        Me.strtBtn.UseVisualStyleBackColor = False
        '
        'progress
        '
        Me.progress.BackColor = System.Drawing.Color.FromArgb(CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer))
        Me.progress.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(121, Byte), Integer), CType(CType(203, Byte), Integer))
        Me.progress.Location = New System.Drawing.Point(13, 406)
        Me.progress.Name = "progress"
        Me.progress.Size = New System.Drawing.Size(736, 44)
        Me.progress.TabIndex = 19
        '
        'backgroundWorker1
        '
        Me.backgroundWorker1.WorkerReportsProgress = True
        '
        'showinfo
        '
        Me.showinfo.BackColor = System.Drawing.Color.FromArgb(CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer), CType(CType(17, Byte), Integer))
        Me.showinfo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.showinfo.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(121, Byte), Integer), CType(CType(203, Byte), Integer))
        Me.showinfo.Location = New System.Drawing.Point(13, 362)
        Me.showinfo.Name = "showinfo"
        Me.showinfo.ReadOnly = True
        Me.showinfo.Size = New System.Drawing.Size(124, 20)
        Me.showinfo.TabIndex = 21
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(121, Byte), Integer), CType(CType(203, Byte), Integer))
        Me.Label1.Location = New System.Drawing.Point(378, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(135, 13)
        Me.Label1.TabIndex = 22
        Me.Label1.Text = "Helbreath Server Launcher"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = CType(resources.GetObject("PictureBox1.Image"), System.Drawing.Image)
        Me.PictureBox1.Location = New System.Drawing.Point(3, 3)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(24, 24)
        Me.PictureBox1.TabIndex = 23
        Me.PictureBox1.TabStop = False
        '
        'closeBtn
        '
        Me.closeBtn.BackColor = System.Drawing.Color.Transparent
        Me.closeBtn.FlatAppearance.BorderSize = 0
        Me.closeBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.closeBtn.Image = Global.Launcher.My.Resources.Resources.close1
        Me.closeBtn.Location = New System.Drawing.Point(877, 1)
        Me.closeBtn.Name = "closeBtn"
        Me.closeBtn.Size = New System.Drawing.Size(34, 26)
        Me.closeBtn.TabIndex = 17
        Me.closeBtn.UseVisualStyleBackColor = False
        '
        'minimizeBtn
        '
        Me.minimizeBtn.BackColor = System.Drawing.Color.Transparent
        Me.minimizeBtn.FlatAppearance.BorderSize = 0
        Me.minimizeBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.minimizeBtn.Image = Global.Launcher.My.Resources.Resources.minimize1
        Me.minimizeBtn.Location = New System.Drawing.Point(837, 1)
        Me.minimizeBtn.Name = "minimizeBtn"
        Me.minimizeBtn.Size = New System.Drawing.Size(34, 26)
        Me.minimizeBtn.TabIndex = 16
        Me.minimizeBtn.UseVisualStyleBackColor = False
        '
        'Launcher
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.FromArgb(CType(CType(72, Byte), Integer), CType(CType(72, Byte), Integer), CType(CType(72, Byte), Integer))
        Me.BackgroundImage = CType(resources.GetObject("$this.BackgroundImage"), System.Drawing.Image)
        Me.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch
        Me.ClientSize = New System.Drawing.Size(916, 470)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.showinfo)
        Me.Controls.Add(Me.progress)
        Me.Controls.Add(Me.strtBtn)
        Me.Controls.Add(Me.closeBtn)
        Me.Controls.Add(Me.minimizeBtn)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(916, 470)
        Me.MinimumSize = New System.Drawing.Size(916, 470)
        Me.Name = "Launcher"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents minimizeBtn As System.Windows.Forms.Button
    Private WithEvents closeBtn As System.Windows.Forms.Button
    Private WithEvents strtBtn As System.Windows.Forms.Button
    Private WithEvents progress As System.Windows.Forms.ProgressBar
    Private WithEvents backgroundWorker1 As System.ComponentModel.BackgroundWorker
    Friend WithEvents BackgroundWorker2 As System.ComponentModel.BackgroundWorker
    Friend WithEvents showinfo As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox

End Class
