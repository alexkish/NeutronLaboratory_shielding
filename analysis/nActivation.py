########################################################################
#
# Get frequency of produced isotopes due to neutrons
# Relevant for us are just neutrons due to neutron Capture
#
########################################################################

import sys
#import matplotlib
from numpy import loadtxt, array, lexsort
from pylab import figure, subplot, suptitle, plot, show, xticks, yticks, xlim, ylim, xlabel, ylabel, yscale, title, text, savefig, bar, subplots_adjust, grid
from collections import defaultdict

#from ROOT import *
from math import sqrt, pi, fabs
from array import array
from operator import add

#filename = '/Users/alexkish1/Documents/darkside/GEANT4/NeutronLabUHM/NeutronLabUHM_idl/macros/activation_SteelWalls_1e6.iso'
filename = '/Users/alexkish1/Documents/darkside/GEANT4/NeutronLabUHM/NeutronLabUHM_idl/macros/activation_SteelBlock_1e6.iso'

#output = './plots/activation_SteelWalls_1e6'
#output = './plots/activation_SteelBlock_1e6_nCapture'
output = './plots/activation_SteelBlock_1e6_nInelastic'
output_scaled = '%s_scaled' % (output)

txt_output_name = '%s.txt' % (output)
html_output_name = '%s.html' % (output)
plot_output_name = '%s.png' % (output)

txt_output_scaled = '%s.txt' % (output_scaled)
html_output_scaled = '%s.html' % (output_scaled)
plot_output_scaled = '%s.png' % (output_scaled)


NeutronRate = 1e6
SimEvents = 1e6
#Time = 3.0 * 86400
#Time = 86400
Time = 3600
NeutronsPerYear = NeutronRate * Time
NeutronsPerDay = NeutronRate * Time
ScaleFactor = NeutronsPerDay/SimEvents
NoScaling = 1.0


########################################################################
# FUNCTION TO PLOT DATA IN BAR PLOT
########################################################################

def plot_bar(data,out_name,ScaleFactor):

    ind = lexsort( [[k[0][0] for k in data.itervalues()],
                    [k[0][1] for k in data.itervalues()]] )

    x = [ data.keys()[i] for i in ind ]
    y = [ len(data[k]) for k in x ]
    y_scaled = [ len(data[k])*ScaleFactor for k in x ]

    #xoffs = array( range( len(data) ) )+0.4
    barwidth = 0.7
    
    figure(figsize=(13,6))    
    suptitle ('Isotopes produced during Am-Be run')
    subplots_adjust(top=0.93, bottom=0.18, right=0.98, left=0.055, wspace=0.17)

    totplot = subplot(1,2,1)
    totplot.yaxis.grid(color='gray', linestyle='solid', zorder='1')
    bar(xoffs, y, width=barwidth, log=True, zorder='2')
    xticks(xoffs+ barwidth/2, x, rotation=-90)

    bar(xoffs, y, width=barwidth, log=True, zorder='2')
    xticks(xoffs+ barwidth/2, x, rotation=-90)

    xlim(0,len(ind)+0.8)
    ylabel('number')
    
    normplot = subplot(1,2,2)
    normplot.yaxis.grid(color='gray', linestyle='solid', zorder='1')
    bar(xoffs, y_scaled, width=barwidth, log=True, zorder='2')
    xticks(xoffs+ barwidth/2, x, rotation=-90)
    xlim(0,len(ind)+0.8)
    ylabel('Production rate [1/day]')

    savefig(out_name)
    show()            



########################################################################
# TEXT OUTPUT
########################################################################

def Gen_txt_output(f, data, volume_dict, ScaleFactor):

    ind = lexsort( [[k[0][0] for k in data.itervalues()],
                    [k[0][1] for k in data.itervalues()]] )

    for k in [data.keys()[i] for i in ind]:
        v = volume_dict[k]
        print >> f, '-' * 80
        print >> f, k
        for name,count in v.iteritems():
            countnorm = count*ScaleFactor
            print >>f,  '    %45s : %6.2f' % (name, countnorm)



########################################################################
# HTML OUTPUT
########################################################################

def Gen_html_output(f, data, volume_dict, ScaleFactor):

    ind = lexsort( [[k[0][0] for k in data.itervalues()],
                    [k[0][1] for k in data.itervalues()]] )


    print >> f, '<p><table align="center" width="75%" border=0 rules=groups frame=box cellpadding=1 bgcolor="#DDDDDD">'
    
    for k in [data.keys()[i] for i in ind]:
        v = volume_dict[k]
        A = data[k][0][0]
        Z = data[k][0][1]

        print >> f, '<tbody>'
        print >> f, '<tr> \n<td><a href="http://nucleardata.nuclear.lu.se/toi/nuclide.asp?iZA=%s00%s"><b> %s</b></a></td><td></td> \n</tr>' % (Z, A, k)
        print >> f, '<tr> \n<td>Half life:</td> \n<td align="right"></td> \n</tr>'
        print >> f, '<tr> \n<td> Decay:</td> \n<td align="right"></td> \n</tr>'
        print >> f, '</tbody> \n<tbody>'
        for name,count in v.iteritems():
            countnorm = count*ScaleFactor
            print >>f,  '<tr>\n<td> %s </td> \n<td align="right"> %.2f </td> \n</tr>' % (name, countnorm)
        print >> f, '</tbody>'

    print >> f, '</table></p>'


########################################################################
# GET DATA AND PRODUCE NECESSARY DICTIONARIES
########################################################################

# Get data and move them into a dictionary with isotopes as keys
data = defaultdict(list)
with open(filename) as f:
    for i,line in enumerate(f):
        if (i%1) == 0: print "\rLine", i,
        line = line.split()
        #if line[-1] == 'nCapture':
        if line[-1] == 'neutronInelastic':
        #if line[-1] == 'HadronElastic':
            line[0] = int(line[0])
            line[1] = int(line[1])
            data[ line[2] ].append(line)
print



# Generate a second dictionary based on the first one to get volumes
# in which the different isotopes are generated
volCounts = defaultdict(dict)
for k in data.keys():
    volCounts[k] = defaultdict(int)
    for d in data[k]:
        a = d[-2]
        b = a[:a.rfind('_')] if a.rfind('_') != -1 else a
        volCounts[k][b] += 1



##########################################################################
# OUTPUT
##########################################################################
#
# Output is produced as table, in html format to simplify the publication
# of the results as well as bar plot. All three show once the original
# data (NoScaling) as well as normalized (ScaleFactor) according to 
# neutron rate given above.
#
##########################################################################


# Write table with isotopes and volumes in which they are generated
# 'Human' readable txt file
#f = sys.stdout
f = open(txt_output_name, 'w')
Gen_txt_output(f,data,volCounts,NoScaling)
f.close

# Same normalized
#g = sys.stdout
g = open(txt_output_scaled, 'w')
Gen_txt_output(g,data,volCounts,ScaleFactor)
g.close


# Generate html output
#f = sys.stdout
f = open(html_output_name, 'w')
Gen_html_output(f,data, volCounts,NoScaling)
f.close

# Same normalized
#g = sys.stdout
g = open(html_output_scaled, 'w')
Gen_html_output(g,data, volCounts,ScaleFactor)
g.close

# Plot isotope abundance 
#plot_bar(data,plot_output_name,ScaleFactor)






      
