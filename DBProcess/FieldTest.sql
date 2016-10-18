CREATE DATABASE MyDB
GO

USE [MyDB]
GO

/****** Object:  Table [dbo].[FieldTest]    Script Date: 2016/10/17 22:50:21 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[FieldTest](
	[ID] [int] NOT NULL IDENTITY(1,1) PRIMARY KEY,
	[int1] [int] NULL,
	[int2big] [bigint] NULL,
	[int3small] [smallint] NULL,
	[int4tiny] [tinyint] NULL,
	[str1vchar] [varchar](50) NULL,
	[str2char] [char](10) NULL,
	[str3text] [text] NULL,
	[dig1num] [numeric](10, 8) NULL,
	[dig2dec] [decimal](20, 8) NULL,
	[dig3float] [float] NULL,
	[time1date] [date] NULL,
	[time2dt] [datetime] NULL,
	[time3stamp] [timestamp] NULL,
	[guid] [uniqueidentifier] NULL,
	[bin1] [binary](50) NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO

TRUNCATE table FieldTest
GO

SET ANSI_PADDING OFF
GO

ALTER TABLE [dbo].[FieldTest] ADD  CONSTRAINT [DF_FieldTest_guid]  DEFAULT (newid()) FOR [guid]
GO

ALTER TABLE [dbo].[FieldTest] ADD  CONSTRAINT [DF_FieldTest_time1date]  DEFAULT (getdate()) FOR [time1date]
GO

ALTER TABLE [dbo].[FieldTest] ADD  CONSTRAINT [DF_FieldTest_time2dt]  DEFAULT (getdate()) FOR [time2dt]
GO

SELECT * FROM FieldTest

