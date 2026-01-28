from django.http import HttpResponse
from django.shortcuts import render

def AboutUs(request):
    return render(request,"aboutus.html")


def Home(request):
    return render(request,"home.html")

def recap(request):
    return render(request,'recape.html')
