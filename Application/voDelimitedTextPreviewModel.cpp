
//QT includes
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTemporaryFile>
#include <QTextStream>

//VTK includes
#include <vtkDelimitedTextReader.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkTable.h>
#include <vtkVariantArray.h>

// Visomics includes
#include "voDelimitedTextPreviewModel.h"


class voDelimitedTextPreviewModelPrivate //: public Ui_voDelimitedTextPreviewModel
{
  Q_DECLARE_PUBLIC(voDelimitedTextPreviewModel);

public:
  voDelimitedTextPreviewModelPrivate(voDelimitedTextPreviewModel& object);
  void init();

  QTemporaryFile SampleCacheFile;

  QString FileName;
  char FieldDelimiter;
  char StringBeginEndCharacter; // Value of 0 indicates none
  bool UseFirstLineAsAttributeNames;
  bool Transpose;
  int HeaderColumnNumber;
  int HeaderRowNumber;
  int PreviewColumnNumber; // Number of columns to skip
  int PreviewRowNumber; // Number of rows to skip
  bool InlineUpdate;


private:
  voDelimitedTextPreviewModel* const q_ptr;
};

// --------------------------------------------------------------------------
// voDelimitedTextPreviewModelPrivate methods

// --------------------------------------------------------------------------
voDelimitedTextPreviewModelPrivate::voDelimitedTextPreviewModelPrivate(voDelimitedTextPreviewModel& object)
  : q_ptr(&object)
{
}

void voDelimitedTextPreviewModelPrivate::init()
{
  FieldDelimiter = ',';
  StringBeginEndCharacter = '\"';
  UseFirstLineAsAttributeNames = true;
  Transpose = false;
  HeaderColumnNumber = 0;
  HeaderRowNumber = 0;
  PreviewColumnNumber = 0;
  PreviewRowNumber = 0;
  InlineUpdate = false;

  // If init() fails, SampleCacheFile will stay closed
  if (this->SampleCacheFile.isOpen())
    {
    this->SampleCacheFile.close();
    }

  // Open real file
  if (this->FileName == QString::null)
    {
    qWarning("QViewsChooseFileFormatWidget: No filename.  Cannot build file preview.");
    return;
    }
  QFile infile(this->FileName);
  bool openStatus = infile.open(QIODevice::ReadOnly);
  if (!openStatus)
    {
    qWarning() << QObject::tr("The file ") << this->FileName << QObject::tr(" could not be opened for reading.  Did something change between when you selected the file and now?");
    return;
    }

  // Read file
  QTextStream instream(&infile);
  QStringList sampleLinesList;
  for (int i = 0; i < NUM_FILE_LINES_READ && !instream.atEnd(); i++)
    {
    QString nextLine = instream.readLine();
    sampleLinesList.push_back(nextLine);
    }

  // Push lines to temp file
  QString sampleLines;
  sampleLines = sampleLinesList.join("\n");
  bool status = this->SampleCacheFile.open();
  if (!status)
    {
    qWarning() << QObject::tr("Couldn't open temporary file for preview!");
    return;
    }
  this->SampleCacheFile.write(sampleLines.toAscii());
}

// --------------------------------------------------------------------------
// voDelimitedTextPreviewModel methods

// --------------------------------------------------------------------------
voDelimitedTextPreviewModel::voDelimitedTextPreviewModel(const QString& newFileName, QObject* newParent) :
  Superclass(newParent), d_ptr(new voDelimitedTextPreviewModelPrivate(*this))
{
  Q_D(voDelimitedTextPreviewModel);
  d->FileName = newFileName;
  d->init();
}

// --------------------------------------------------------------------------
voDelimitedTextPreviewModel::~voDelimitedTextPreviewModel()
{
}

// --------------------------------------------------------------------------
QString voDelimitedTextPreviewModel::getFileName() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->FileName;
}

void voDelimitedTextPreviewModel::setFileName(const QString& _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->FileName != _arg)
    {
    d->FileName = _arg;
    d->init(); // Different behavior from other accessors
    }
}

// --------------------------------------------------------------------------
char voDelimitedTextPreviewModel::getFieldDelimiter() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->FieldDelimiter;
}

void voDelimitedTextPreviewModel::setFieldDelimiter (char _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->FieldDelimiter != _arg)
    {
    d->FieldDelimiter = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
char voDelimitedTextPreviewModel::getStringBeginEndCharacter() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->StringBeginEndCharacter;
}

void voDelimitedTextPreviewModel::setStringBeginEndCharacter(char _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->StringBeginEndCharacter != _arg)
    {
    d->StringBeginEndCharacter = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
bool voDelimitedTextPreviewModel::getUseFirstLineAsAttributeNames() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->UseFirstLineAsAttributeNames;
}

void voDelimitedTextPreviewModel::setUseFirstLineAsAttributeNames(bool _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->UseFirstLineAsAttributeNames != _arg)
    {
    d->UseFirstLineAsAttributeNames = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
bool voDelimitedTextPreviewModel::getTranspose() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->Transpose;
}

void voDelimitedTextPreviewModel::setTranspose(bool _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->Transpose != _arg)
    {
    d->Transpose = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
int voDelimitedTextPreviewModel::getHeaderColumnNumber() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->HeaderColumnNumber;
}

void voDelimitedTextPreviewModel::setHeaderColumnNumber(int _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->HeaderColumnNumber != _arg)
    {
    d->HeaderColumnNumber = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
int voDelimitedTextPreviewModel::getHeaderRowNumber() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->HeaderRowNumber;
}

void voDelimitedTextPreviewModel::setHeaderRowNumber(int _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->HeaderRowNumber != _arg)
    {
    d->HeaderRowNumber = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
int voDelimitedTextPreviewModel::getPreviewColumnNumber() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->PreviewColumnNumber;
}

void voDelimitedTextPreviewModel::setPreviewColumnNumber(int _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->PreviewColumnNumber != _arg)
    {
    d->PreviewColumnNumber = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
int voDelimitedTextPreviewModel::getPreviewRowNumber() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->PreviewRowNumber;
}

void voDelimitedTextPreviewModel::setPreviewRowNumber(int _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->PreviewRowNumber != _arg)
    {
    d->PreviewRowNumber = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}
// --------------------------------------------------------------------------
bool voDelimitedTextPreviewModel::getInlineUpdate() const
{
  Q_D(const voDelimitedTextPreviewModel);
  return d->InlineUpdate;
}

void voDelimitedTextPreviewModel::setInlineUpdate(bool _arg)
{
  Q_D(voDelimitedTextPreviewModel);
  if (d->InlineUpdate != _arg)
    {
    d->InlineUpdate = _arg;
    if (d->InlineUpdate)
      {
      this->updatePreview();
      }
    }
}


void voDelimitedTextPreviewModel::updatePreview()
{
  Q_D(voDelimitedTextPreviewModel);

  if (d->SampleCacheFile.fileName()=="")
  {
  qWarning() << QObject::tr("ERROR: updatePreview: SampleCacheFile filename not set");
  return;
  }

  // Set up vtkDelimitedTextReader
  vtkSmartPointer<vtkDelimitedTextReader> previewReader = vtkSmartPointer<vtkDelimitedTextReader>::New();

  previewReader->DetectNumericColumnsOff();

  previewReader->SetFileName(d->SampleCacheFile.fileName().toAscii().data());

  char delim_string[2];
  delim_string[0] = d->FieldDelimiter;
  delim_string[1] = 0;
  vtkUnicodeString delimiters = vtkUnicodeString::from_utf8(delim_string);
  previewReader->SetUnicodeFieldDelimiters(delimiters);
  if (d->FieldDelimiter == ' ' || d->FieldDelimiter == '\t')
    {
    previewReader->SetMergeConsecutiveDelimiters(true);
    }
  else
    {
    previewReader->SetMergeConsecutiveDelimiters(false);
    }


  if (d->StringBeginEndCharacter) // Value of 0 indicates none
    {
    previewReader->SetStringDelimiter(d->StringBeginEndCharacter);
    previewReader->SetUseStringDelimiter(true);
    }
  else
    {
    previewReader->SetUseStringDelimiter(false);
    }

  previewReader->SetHaveHeaders(d->UseFirstLineAsAttributeNames);

  // Read in file to table
  previewReader->Update();
  vtkSmartPointer<vtkTable> table = previewReader->GetOutput();

  if (d->Transpose) // Assumes there is a header column ... which we have no setting to specify for anyway
    {
    vtkSmartPointer<vtkTable> transposeTable = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkStringArray> header = vtkSmartPointer<vtkStringArray>::New();
    header->SetName("header");
    header->SetNumberOfTuples(table->GetNumberOfColumns()-1);
    for (vtkIdType c = 1; c < table->GetNumberOfColumns(); ++c)
      {
      header->SetValue(c-1, table->GetColumnName(c));
      }
    transposeTable->AddColumn(header);
    for (vtkIdType r = 0; r < table->GetNumberOfRows(); ++r)
      {
      vtkSmartPointer<vtkStringArray> newcol = vtkSmartPointer<vtkStringArray>::New();
      newcol->SetName(table->GetValue(r, 0).ToString().c_str());
      newcol->SetNumberOfTuples(table->GetNumberOfColumns() - 1);
      for (vtkIdType c = 1; c < table->GetNumberOfColumns(); ++c)
        {
        newcol->SetValue(c-1, table->GetValue(r, c).ToString());
        }
      transposeTable->AddColumn(newcol);
      }
    table = transposeTable;
    }

  // Build model (self)
  this->clear();

  for (vtkIdType r = d->PreviewRowNumber; r < table->GetNumberOfRows(); r++)
    {
    if (r == d->HeaderRowNumber)
      {
      this->setHeaderData(static_cast<int>(r), Qt::Horizontal, QString(table->GetValue(r, 0).ToString()));
      continue;
      }
    QList<QStandardItem *> itemsInRow;
    vtkVariantArray *row = table->GetRow(r);
    for (vtkIdType c = d->PreviewColumnNumber; c < table->GetNumberOfColumns(); c++)
      {
      // Not compatible with having named headers, will fix when we add metadata tables
/*
      if (c == d->HeaderColumnNumber)
        {
        this->setHeaderData(static_cast<int>(r), Qt::Horizontal, QString(table->GetValue(r, c).ToString()));
        }
*/
      if (r == d->HeaderRowNumber)
        {
        this->setHeaderData(static_cast<int>(c), Qt::Vertical, QString(table->GetValue(r, c).ToString()));
        }
      else
        {
      QStandardItem *textItem = new QStandardItem;
      textItem->setText(row->GetValue(c).ToString().c_str());
      itemsInRow.push_back(textItem);
        }
      }
    this->appendRow(itemsInRow);
    }

  // Should discontinue this in favor of metadata tables
  QStringList headerLabels;
  for (vtkIdType c = d->PreviewColumnNumber; c < table->GetNumberOfColumns(); ++c)
    {
    headerLabels.push_back(QString(table->GetColumnName(c)));
    }
  this->setHorizontalHeaderLabels(headerLabels);

}

