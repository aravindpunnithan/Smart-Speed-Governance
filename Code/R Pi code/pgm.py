import requests
from bs4 import BeautifulSoup

lat =  76.617163
lng = 9.317420
N = 0.001
a = lat - (0.009 * N)
b = lat + (0.009 * N)
c = lng - (0.009 * N)
d = lng + (0.009 * N)

r = requests.get('http://www.overpass-api.de/api/xapi?*[maxspeed:motorcycle][bbox={},{},{},{}]'.format(a,c,b,d) )

y=BeautifulSoup(r.text)

for i in  y.findAll("way")[0].findAll("tag"):
    if i["k"] == "maxspeed":
        print "Max Speed is {}".format(i["v"])