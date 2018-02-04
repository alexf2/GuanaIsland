#include "stdafx.h"
#include "CRHelpers.h"

void SetLogOnInfoTo(CRPEJob* pJob, const CRPELogOnInfo* ploi)
{
	// Setting logon info
	short nTables = pJob->GetNTables();
	while (nTables--)
		pJob->SetNthTableLogonInfo(nTables, ploi, FALSE);
	short nSections = pJob->GetNSections();
	while (nSections--) {
		short SectionCode = pJob->GetSectionCode(nSections);
		short nSubReports = pJob->GetNSubreportsInSection(SectionCode);
		while (nSubReports--) {
			DWORD hSubReport = pJob->GetNthSubreportInSection(SectionCode, nSubReports);
			CRPESubreportInfo sri;
			pJob->GetSubreportInfo(hSubReport, &sri);
			CRPEJob* pSubReport = pJob->OpenSubreportJob(sri.m_name);
			short nTables = pSubReport->GetNTables();
			while (nTables--)
				pSubReport->SetNthTableLogonInfo(nTables, ploi, FALSE);
			pSubReport->Close();
		}
	}
}
