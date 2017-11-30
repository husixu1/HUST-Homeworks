/***********************************************************************
*    NDDIMS - Natural Disasters Data and Information Management System
*    Copyright (C) 2016  HuSixu	 Email:husixu1@hotmail.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include "file_operation_xml.h"
#include "node_operation.h"
#include "data_structure.h"
#include <stdio.h>
#include <string.h>
#include <gtk.h>

static const char xml_head[3000] =
"<?xml version=\"1.0\"?>\n"
"<?mso-application progid=\"Excel.Sheet\"?>\n"
"<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n"
" xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n"
" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n"
" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n"
" xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n"
" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n"
"  <Author>HuSixu</Author>\n"
"  <LastAuthor>HuSixu</LastAuthor>\n"
"  <Version>15.00</Version>\n"
" </DocumentProperties>\n"
" <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n"
"  <AllowPNG/>\n"
" </OfficeDocumentSettings>\n"
" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n"
"  <WindowHeight>12450</WindowHeight>\n"
"  <WindowWidth>28800</WindowWidth>\n"
"  <WindowTopX>0</WindowTopX>\n"
"  <WindowTopY>0</WindowTopY>\n"
"  <ProtectStructure>False</ProtectStructure>\n"
"  <ProtectWindows>False</ProtectWindows>\n"
" </ExcelWorkbook>\n"
" <Styles>\n"
"  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n"
"   <Alignment ss:Vertical=\"Center\"/>\n"
"   <Borders/>\n"
"   <Interior/>\n"
"   <NumberFormat/>\n"
"   <Protection/>\n"
"  </Style>\n"
"  <Style ss:ID=\"s27\">\n"
"   <Interior ss:Color=\"#FFC7CE\" ss:Pattern=\"Solid\"/>\n"
"  </Style>\n"
"  <Style ss:ID=\"s26\">\n"
"   <Interior ss:Color=\"#C6EFCE\" ss:Pattern=\"Solid\"/>\n"
"  </Style>\n"
"  <Style ss:ID=\"s28\">\n"
"   <Interior ss:Color=\"#FFEB9C\" ss:Pattern=\"Solid\"/>\n"
"  </Style>\n"
" </Styles>\n"
" <Worksheet ss:Name=\"Data\">\n"
"  <Table ss:ExpandedColumnCount=\"17\" ss:ExpandedRowCount=\"00000000\" x:FullColumns=\"1\""
"  x:FullRows=\"1\" ss:DefaultColumnWidth=\"70\" ss:DefaultRowHeight=\"13.5\">\n"
"   <Row>\n"
"    <Cell><Data ss:Type=\"String\">Node type</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Disaster type</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Disaster name</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event Disaster name</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event serial</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event time</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event level</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event location</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event deads</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event house destroyed</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Event pecuniary loss</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donate Disaster name</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donate Event serial</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donor's name</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donate time</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donate type</Data></Cell>\n"
"    <Cell><Data ss:Type=\"String\">Donate amount</Data></Cell>\n"
"   </Row>\n";

static const char xml_tail[1000]=
"  </Table>\n"
"  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n"
"   <PageSetup>\n"
"    <Header x:Margin=\"0.3\"/>\n"
"    <Footer x:Margin=\"0.3\"/>\n"
"    <PageMargins x:Bottom=\"0.75\" x:Left=\"0.7\" x:Right=\"0.7\" x:Top=\"0.75\"/>\n"
"   </PageSetup>\n"
"   <Selected/>\n"
"   <Panes>\n"
"    <Pane>\n"
"     <Number>3</Number>\n"
"     <ActiveRow>12</ActiveRow>\n"
"     <ActiveCol>11</ActiveCol>\n"
"     <RangeSelection>R13C12:R13C17</RangeSelection>\n"
"    </Pane>\n"
"   </Panes>\n"
"   <ProtectObjects>False</ProtectObjects>\n"
"   <ProtectScenarios>False</ProtectScenarios>\n"
"  </WorksheetOptions>\n"
" </Worksheet>\n"
"</Workbook>\n";

error_t file_save_as_xml(const char *file_name){
	Node *temp1;	//node_type_classify
    Node *temp2;	//node_type_event
    Node *temp3;	//node_type_donate
    unsigned long counter=0;
	/* save into file*/
	FILE *f_temp	=	fopen(file_name,"w");
	fprintf(f_temp,xml_head);
    for(temp1=head_classify->next;temp1!=NULL;temp1=temp1->next){
		fprintf(f_temp,
			"   <Row>\n"
			"    <Cell><Data ss:Type=\"String\">classify</Data></Cell>\n"
			"    <Cell ss:StyleID=\"s27\"><Data ss:Type=\"String\">%c</Data></Cell>\n"
			"    <Cell ss:StyleID=\"s27\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
			"   </Row>\n",
			type_classify(temp1),
			name_classify(temp1));
			counter++;
		for(temp2=temp1->child;temp2!=NULL;temp2=temp2->next){
			fprintf(f_temp,
				"   <Row>\n"
				"    <Cell><Data ss:Type=\"String\">event</Data></Cell>\n"
				"    <Cell ss:Index=\"4\" ss:StyleID=\"s26\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%d</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%d</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%d</Data></Cell>\n"
				"    <Cell ss:StyleID=\"s26\"><Data ss:Type=\"String\">%f</Data></Cell>\n"
				"   </Row>\n",
				name_event(temp2),
				serial_event(temp2),
				time_event(temp2),
				level_event(temp2),
				location_event(temp2),
				deads_event(temp2),
				house_destroyed_event(temp2),
				pencuniary_loss_event(temp2));
				counter++;
			for(temp3=temp2->child;temp3!=NULL;temp3=temp3->next){
				fprintf(f_temp,
					"   <Row>\n"
					"    <Cell><Data ss:Type=\"String\">donate</Data></Cell>\n"
					"    <Cell ss:Index=\"12\" ss:StyleID=\"s28\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
					"    <Cell ss:StyleID=\"s28\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
					"    <Cell ss:StyleID=\"s28\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
					"    <Cell ss:StyleID=\"s28\"><Data ss:Type=\"String\">%s</Data></Cell>\n"
					"    <Cell ss:StyleID=\"s28\"><Data ss:Type=\"String\">%c</Data></Cell>\n"
					"    <Cell ss:StyleID=\"s28\"><Data ss:Type=\"String\">%f</Data></Cell>\n"
					"   </Row>\n",
					classify_name_donate(temp3),
					serial_donate(temp3),
					name_donate(temp3),
					time_donate(temp3),
					type_donate(temp3),
					amount_donate(temp3));
					counter++;
			}
		}
	}
	fprintf(f_temp,xml_tail);
	rewind(f_temp);
	/* set the line number of the xml file */
	fseek(f_temp,1487,SEEK_SET);
	counter+=2;
	char buff[9]="00000000";
	for(int i=0;counter!=0;counter/=10,i++)
		buff[7-i]	=	counter%10+'0';
	fprintf(f_temp,buff);
	fclose(f_temp);
	return no_error;
}
