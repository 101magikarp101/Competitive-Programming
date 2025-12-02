#!/bin/sh
echo Starting Codeforces Contest Wizard...
name=contest-wizard-starter-1.0-`date +%N`.jar
url=http://codeforces.org/s/10400/wizard/contest-wizard-starter-1.0-SNAPSHOT.jar?session=6b462f692c931fde2a70906c8e111316
cd $TEMP
wget --output-document=$name $url
java -cp $name -Xss5m -Xmx1024m com.codeforces.contestwizard.Main VVQgMTA0YyBNaWR0ZXJtICMy \# NTA MTA2MTUyL3NhbmRib3gvY29udGVzdC56aXA Km03YTFnNWk4azhhMXI0cDA YnJfUjFjTU1jeV94V0RRSnA1WmhNMk53T3hJMDFGQzdlaHNiY2JuWUJzMGtDeXBoUQ ZW4 dHJ1ZQ Rg \# \# &
