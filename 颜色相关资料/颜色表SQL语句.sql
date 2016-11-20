USE [MyDB]
GO

/****** Object:  Table [dbo].[ChinaColor]    Script Date: 2016/10/28 17:19:17 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[ChinaColor](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[name] [varchar](50) NULL,
	[HEX] [varchar](50) NULL,
	[red] [int] NULL,
	[green] [int] NULL,
	[blue] [int] NULL,	
	[HEXred] [varchar](10) NULL,
	[HEXgreen] [varchar](10) NULL,
	[HEXblue] [varchar](10) NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO

--��������1-ȡ3��ɫHEXֵ
update ChinaColor set HEXred = SUBSTRING(hex, 2,2)
update ChinaColor set HEXgreen = SUBSTRING(hex, 4,2)
update ChinaColor set HEXblue = SUBSTRING(hex, 6,2)

 --16�����ַ���ת10����int��0-FFFFFFFFFFFFFFFF����
 CREATE  function [hextoint](@s varchar(10)) 
returns int  
begin 
 declare @result int
 set @result=CONVERT(int, CONVERT(varbinary, CAST(N'0x' + @s AS char), 1))--�����Ч�ķ���
 return @result
END

--��������2-ȡ3��ɫintֵ
update ChinaColor set red = dbo.hextoint(HEXred)
update ChinaColor set green = dbo.hextoint(HEXgreen)
update ChinaColor set blue = dbo.hextoint(HEXblue)

--������ɫ
--��ɫ
select name,HEX from ChinaColor 
where red > 200 
		and green + blue < 200

order by red 

--��ɫ
select name,HEX from ChinaColor 
where green > 200 
		and red + blue < 200
	
order by green 

--��ɫ
select name,HEX from ChinaColor 
where blue > 128 
	and (red + green) < 100
order by blue 

--��ɫ
select name,HEX from ChinaColor 
where abs((red - green)) < 33
	and (red > 128 or green > 128 )
	and blue < 100
order by red + green