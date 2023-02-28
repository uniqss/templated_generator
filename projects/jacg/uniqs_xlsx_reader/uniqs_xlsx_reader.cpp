#include "uniqs_xlsx_reader.h"

#include <OpenXLSX.hpp>
#include <iostream>
#include <numeric>
#include <random>

using namespace std;
using namespace OpenXLSX;

char __transCellType(const OpenXLSX::XLValueType& vt) {
    // Empty, Boolean, Integer, Float, Error, String
    static char type[] = {'N', 'B', 'I', 'F', 'E', 'S'};
    return type[(int)vt];
}

void __getCellValueString(const OpenXLSX::XLCellValueProxy& cellValue, std::string& str) {
    switch (cellValue.type()) {
        case XLValueType::String:
            str = cellValue.get<std::string>();
            break;
        case XLValueType::Integer:
            str = std::to_string(cellValue.get<int64_t>());
            break;
        case XLValueType::Empty:
            str = "";
            break;
        case XLValueType::Float:
            str = std::to_string(cellValue.get<double>());
            break;
        default:
            str = "";
            break;
    }
}

int read_xlsx(const char* filename, std::vector<std::pair<std::string, std::vector<std::vector<std::string> > > >& vecResult, int maxRows, int maxCols) {
    XLDocument doc;
    OpenXLSX::XLWorksheet wks;

    std::string filaNameStr = filename;
    doc.open(filaNameStr);
    std::vector<std::string> sheetNames = doc.workbook().sheetNames();

    vecResult.clear();

    uint32_t rowIdxMax = maxRows;
    if (maxRows <= 0) {
        rowIdxMax = std::numeric_limits<unsigned>::max();
    }
    uint32_t colIdxMax = maxCols;
    if (maxCols <= 0) {
        colIdxMax = std::numeric_limits<unsigned>::max();
    }

    for (auto sheetName : sheetNames) {
        std::pair<std::string, std::vector<std::vector<std::string> > > oneSheet;
        oneSheet.first = sheetName;

        wks = doc.workbook().worksheet(sheetName);

        for (uint32_t rowidx = 1; rowidx < wks.rowCount() + 1 && rowidx <= rowIdxMax; rowidx++) {
            std::vector<string> oneRow;
            auto row = wks.row(rowidx);
            // cout << "rowidx:" << rowidx << " cellCount:" << row.cellCount() << endl;
            for (unsigned colidx = 1; colidx <= row.cellCount() && colidx <= colIdxMax; ++colidx) {
                const OpenXLSX::XLCell& cell = wks.cell(rowidx, colidx);
                const OpenXLSX::XLCellValueProxy& cellValue = cell.value();
                // cout << rowidx << "|" << colidx << " cell.valueType():" << __transCellType(cell.valueType()) << endl;

                std::string oneCell;
                __getCellValueString(cellValue, oneCell);

                oneRow.push_back(oneCell);
            }
            oneSheet.second.push_back(oneRow);
        }
        vecResult.push_back(oneSheet);
    }

    doc.close();

    return 0;
}
int read_xlsx(const std::string& filename, std::vector<std::pair<std::string, std::vector<std::vector<std::string> > > >& vecResult, int maxRows, int maxCols) {
    return read_xlsx(filename.c_str(), vecResult);
}
