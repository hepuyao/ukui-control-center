/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "addshortcut.h"
#include "ui_addshortcut.h"

#define DEFAULTPATH "/usr/share/applications/"

AddShortcut::AddShortcut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddShortcut)
{
    ui->setupUi(this);

    refresh_certainbtn_status();

    connect(ui->openBtn, SIGNAL(clicked(bool)), this, SLOT(open_filedialog_slot()));
    connect(this, &AddShortcut::finished, this, [=]{update_dialog_unset();});
    connect(ui->cancelBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->certainBtn, &QPushButton::clicked, this, [=]{emit program_info_signal(gspath, ui->nameLineEdit->text(), ui->execLineEdit->text()); close();});

    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, [=]{refresh_certainbtn_status();});
    connect(ui->execLineEdit, &QLineEdit::textChanged, this, [=]{refresh_certainbtn_status();});
}

AddShortcut::~AddShortcut()
{
    delete ui;
}

void AddShortcut::refresh_certainbtn_status(){
    if (!ui->nameLineEdit->text().isEmpty() && !ui->execLineEdit->text().isEmpty())
        ui->certainBtn->setEnabled(true);
    else
        ui->certainBtn->setEnabled(false);
}

void AddShortcut::update_dialog_set(QString gsettings_path, QString name, QString exec){
    gspath = gsettings_path;
    ui->nameLineEdit->setText(name);
    ui->execLineEdit->setText(exec);
}

void AddShortcut::update_dialog_unset(){
    gspath = "";
    ui->nameLineEdit->clear();
    ui->execLineEdit->clear();
    ui->nameLineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void AddShortcut::open_filedialog_slot(){
    QString filters = "Desktop files(*.desktop)";
    QFileDialog fd;
    fd.setDirectory(DEFAULTPATH);
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setViewMode(QFileDialog::List);
    fd.setNameFilter(filters);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle(tr("selsect desktop"));
    fd.setLabelText(QFileDialog::Accept, "Select");

    if (fd.exec() != QDialog::Accepted)
        return;

    QString selectedfile;
    selectedfile = fd.selectedFiles().first();

    QString exec = selectedfile.section("/", -1, -1);
    exec.replace(".desktop", "");
    ui->execLineEdit->setText(exec);
}
