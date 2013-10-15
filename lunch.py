import os, sys

lumi = 20000 # // fb-1

def nevent(cross_sec) :
    #print 'cross section' + str(cross_sec)
    return int(lumi * cross_sec)

alist={}
#alist['./data/4f/E250-TDR_ws.P4f_ww_h.Gwhizard-1_95.eL.pR.I106551.002.slcio'] = '14874.28' #! in fb
#alist['./data/higgs/E250-TDR_ws.Pqqh.Gwhizard-1_95.eL.pR.I106485.001.slcio']  = '346.01308' #! in fb

# for test
alist['./data/4f/E250-TDR_ws.P4f_ww_h.Gwhizard-1_95.eL.pR.I106551.002.slcio'] = '14874.28' #! in fb
alist['./data/4f/E250-TDR_ws.P4f_ww_h.Gwhizard-1_95.eL.pR.I106551.002.slcio'] = '' #! in fb
alist['./data/4f/E250-TDR_ws.P4f_ww_h.Gwhizard-1_95.eL.pR.I106551.002.slcio'] = '' #! in fb
alist['./data/higgs/E250-TDR_ws.Pqqh.Gwhizard-1_95.eL.pR.I106485.001.slcio']  = '346.01308' #! in fb


print 'INPUT FILE: ', alist

#cmd = './carefulDownload.sh ' + alist +' ${INPDIR}'
#print cmd
#os.system(cmd)


fuck=0
for key in alist :
    print key
    fuck=fuck+1
    oldXMLname='run.xml'
    oldXML = open(oldXMLname,'r')
    newXMLname='run.%i.xml' % (fuck)
    newXML = open(newXMLname,'w')
    for line in oldXML :
        newline = line
        if 'INPUT' in line :
            newline = '\t'+key+ '\n' 
        if 'NEVT' in line :
            linesp = line.split('NEVT')
            Nevt = nevent(float(alist[key]))
            newline = linesp[0].strip() + str(Nevt) + linesp[1].strip() + '\n'
            print 'file ' + key + '----> ' + str(Nevt) + ' events !!'
            
        if 'HISTFILE' in line :
            linesp = line.split('HISTFILE')
            histname = key.replace("Gwhizard","checkPlot")
            histname = histname.replace("slcio","root")
            newline = linesp[0].strip() + histname + linesp[1].strip() + '\n'
        if 'JETTREE' in line:
            linesp = line.split('JETTREE')
            histname = key.replace("Gwhizard","JetTree")
            histname = histname.replace("slcio","root")
            newline = linesp[0].strip() + histname + linesp[1].strip() + '\n'
        if 'OUTPUTFILE' in line:
            linesp = line.split('OUTPUTFILE')
            histname = key.replace("Gwhizard","Reco")
            newline = linesp[0].strip() + histname + linesp[1].strip() + '\n'
        newXML.write(newline)
	
    oldXML.close()
    newXML.close()
    
    cmd='Marlin ' + newXMLname  
    os.system(cmd)
    
