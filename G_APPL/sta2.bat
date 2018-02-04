set CLASSPATH=C:\
start c:\rmiregistry.exe
java.exe -Djava.rmi.server.hostname=server1 -Djava.rmi.server.codebase=file:/f:\work\G_APPL/ -classpath "F:\WORK\G_APPL\;E:\JBuilder2\lib\swingall.jar;E:\JBuilder2\lib\jbcl2.0.jar;E:\JBuilder2\lib\jbcl2.0-res.jar;E:\JBuilder2\java\lib\classes.zip" G_APPL.GServer gserver=SERVER1
