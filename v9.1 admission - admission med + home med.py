import re
import csv


def med_search(text):
    exclusion = '(?<!continue )(?<!continue your )(?<!continue his )(?<!continue her )(?<!continued )(?<!continued his )(?<!continued her )(?<!hold )(?<!on )'
    med_adm = re.compile(
        'medication[s]?.*admission.*:|admission.*medication[s]?.*:|meds.*admission.*:|admission.* meds.*:|'
        + exclusion + '(home +med.*:|med.+at home.*:|med.+[(]at home[)]|home +med.+include)',
        re.IGNORECASE
    )
    return med_adm.search(text)


def title_search(text, med_end):
    title_regex = re.compile(
        '\n\s*\n.*(discharge medication[s]?|discharge|disposition|allerg|social|history|'
        'allx|assessment|chief|complaint|course|data|diagno|diet|evaluation|exam|family|general|'
        'habit[s]?|hpi|hx|illness|inpatient|issue|laborator|labs|osh|outstanding|'
        'past|pertinent|physical|pmh|presentation|radiology|result[s]?|review|sign|'
        'studies|study|summary|system|vital|x-ray|followup|instruction|status|ccu|dictated by)'
        '.*:'
        '|\n\s*\n.*(gen|genrl|ros):',
        re.IGNORECASE
    )
    bad_title_regex = re.compile('sig:|\d\..*:|mg.*:', re.IGNORECASE)

    title2 = title_regex.search(text, med_end)
    test = bad_title_regex.search(title2.group())
    while test:
        title2 = title_regex.search(text, title2.end())
        test = bad_title_regex.search(title2.group())
    return title2


def txt_btwn(text, med_end, title_start):
    return text[med_end+1:title_start]


def txt_after(text, title_end):
    return text[title_end:title_end+50]


def drug_search(text, med_end, title_start):
    drug = re.compile(
        "Omeprazole|Lansoprazole|Dexlansoprazole|Esomeprazole|Pantoprazole|Rabeprazole|Gasec|Prevacid|Kapidex|Nexium|"
        "Protonix|AcipHex|Losec|Zoton|Dexilant|Esotrex|Somac|Pariet|Prilosec",
        re.IGNORECASE
    )
    drug_list = drug.findall(text, med_end, title_start)
    if len(drug_list) != 0:
        return ', '.join(drug_list)
    else:
        return None


def output(type, row, subject, hadm, chartdate, med, title, drugs, between, after, text):
    print row
    if type == 'csv':
        writer.writerow(
            {
                'ROW_ID': row,
                'SUBJECT_ID': subject,
                'HADM_ID': hadm,
                'CHARTDATE': chartdate,
                'ADMISSION MED TITLE': med,
                'NEXT TITLE': title,
                'DRUGS FOUND': drugs,
                'BETWEEN TITLES': between,
                'AFTER NEXT TITLE': after
            }
        )

    elif type == 'txt':
        file_out = open('C:\\Users\\John\\Desktop\\Note\\Row_ID_' + row + '.txt', "w")
        file_out.write(
            'ROW_ID: ' + row +
            '\nSUBJECT_ID: ' + subject +
            '\nHADM_ID: ' + hadm +
            '\nCHARTDATE: ' + chartdate +
            '\n\nTITLE FOUND: ' + med +
            '\n\nDRUGS FOUND: ' + drugs +
            '\n\nNEXT TITLE FOUND: ' + title +
            '\n\nBETWEEN: \n' + between +
            '\n\nAFTER: \n' + after
        )
        file_out.close()

file_type = "csv"

if file_type == "csv":
    csv_1 = open('C:\Users\John\Desktop\Results.csv', 'wb')
    fieldnames = [
        'ROW_ID', 'SUBJECT_ID', 'HADM_ID', 'CHARTDATE', 'ADMISSION MED TITLE', 'NEXT TITLE', 'DRUGS FOUND',
        'AFTER NEXT TITLE', 'BETWEEN TITLES', 'TEXT'
    ]
    writer = csv.DictWriter(csv_1, fieldnames=fieldnames)
    writer.writeheader()

with open('C:\Users\John\Desktop\MIMIC III\\NOTEEVENTS_1.4.csv') as csv_2:
    reader = csv.DictReader(csv_2)
    for row in reader:
        if str(row['CATEGORY']) == 'Discharge summary':
            txt = str(row['TEXT'])
            med = med_search(txt)
            if med is not None:
                title = title_search(txt, med.end())
                between = txt_btwn(txt, med.end(), title.start())
                after = txt_after(txt, title.end())
                drugs = drug_search(txt, med.end(), title.start())
                output(
                    file_type, row['ROW_ID'], row['SUBJECT_ID'], row['HADM_ID'], row['CHARTDATE'], med.group(),
                    title.group(), drugs, between, after, txt
                )
            else:
                continue
