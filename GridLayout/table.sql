CREATE TABLE uiinfo (
    "WidgetName" TEXT NOT NULL,
    "iDispNo" INTEGER NOT NULL,
    "strUIType" TEXT NOT NULL,
    "strCtrlType" TEXT NOT NULL,
    "strCtrlName" TEXT NOT NULL,
    "strDispName" TEXT,
    "iDRow" INTEGER NOT NULL,
    "iDColumn" INTEGER NOT NULL,
    "iSpanRow" INTEGER NOT NULL DEFAULT (1),
    "iSpanColumn" INTEGER NOT NULL DEFAULT (1),
    "strSignals" TEXT,
    "strSlots" TEXT
)

insert into uiinfo 
select "WidgetName", "iDispNo", "", "strCtrlType", "strCtrlName", "strDispName", "iDRow", "iDColumn", "iSpanRow", "iSpanColumn", "", "" from uiold;

delete from uiinfo where idispno>12
