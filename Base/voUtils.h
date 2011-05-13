#ifndef __voUtils_h
#define __voUtils_h

class vtkAbstractArray;
class vtkStringArray;
class vtkTable;
template <class T> class QList;
class QString;
class vtkArray;
template <class T> class vtkSmartPointer;

namespace voUtils 
{

enum TransposeOption
  {
  WithoutHeaders = 0x0,
  FirstColumnIntoColumnNames = 0x1,
  ColumnNamesIntoFirstColumn = 0x2,
  Headers = 0x3,
  };

bool transposeTable(vtkTable* srcTable, vtkTable* destTable, const TransposeOption& transposeOption = WithoutHeaders);

bool transposeTable(vtkTable* table, const TransposeOption& transposeOption = WithoutHeaders);

bool insertColumnIntoTable(vtkTable * table, int position, vtkAbstractArray * column);

vtkStringArray* tableColumnNames(vtkTable * table);

void setTableColumnNames(vtkTable * table, vtkStringArray * columnNames);

bool parseRangeString(const QString& rangeString, QList<int>& rangeList, bool alpha);

QString counterIntToAlpha(int intVal);

int counterAlphaToInt(const QString& alphaVal);

bool tableToArray(vtkTable* srcTable, vtkSmartPointer<vtkArray>& destArray, const QList<int>& columnList);

void arrayToTable(vtkArray* srcArray, vtkTable* destTable);
}

#endif
