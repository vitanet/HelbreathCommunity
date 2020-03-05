<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Update
    Inherits System.Windows.Forms.Form

    'Form reemplaza a Dispose para limpiar la lista de componentes.
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

    'Requerido por el Diseñador de Windows Forms
    Private components As System.ComponentModel.IContainer

    'NOTA: el Diseñador de Windows Forms necesita el siguiente procedimiento
    'Se puede modificar usando el Diseñador de Windows Forms.  
    'No lo modifique con el editor de código.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Update))
        Me.Console = New System.Windows.Forms.RichTextBox()
        Me.progress = New System.Windows.Forms.ProgressBar()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'Console
        '
        Me.Console.BackColor = System.Drawing.SystemColors.InfoText
        Me.Console.ForeColor = System.Drawing.Color.Green
        Me.Console.Location = New System.Drawing.Point(12, 85)
        Me.Console.Name = "Console"
        Me.Console.ReadOnly = True
        Me.Console.Size = New System.Drawing.Size(517, 202)
        Me.Console.TabIndex = 0
        Me.Console.Text = ""
        '
        'progress
        '
        Me.progress.Location = New System.Drawing.Point(12, 56)
        Me.progress.Name = "progress"
        Me.progress.Size = New System.Drawing.Size(517, 23)
        Me.progress.TabIndex = 1
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(227, 308)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(75, 23)
        Me.Button1.TabIndex = 2
        Me.Button1.Text = "Close"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(191, 19)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(139, 13)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "HelbreathSS Server Update"
        '
        'Update
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(541, 352)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.progress)
        Me.Controls.Add(Me.Console)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximumSize = New System.Drawing.Size(557, 391)
        Me.MinimumSize = New System.Drawing.Size(557, 391)
        Me.Name = "Update"
        Me.Text = "HelbreathSS Server Updater"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Console As System.Windows.Forms.RichTextBox
    Friend WithEvents progress As System.Windows.Forms.ProgressBar
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label

End Class
