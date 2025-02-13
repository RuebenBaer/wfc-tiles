/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul Verkettete Liste

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef __Verkettete_Liste_
#define __Verkettete_Liste_
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

template<class T>  class Listenelement
{
  private:
    T* Element;
    Listenelement* Nachfolger;
    Listenelement* Vorgaenger;
    double m_Wert;
  public:
    Listenelement();
    Listenelement(const Listenelement&);
    Listenelement(T*);
    ~Listenelement();
    const Listenelement& operator=(const Listenelement&) const;
    void SetNachfolger(Listenelement*);
    void SetVorgaenger(Listenelement*);
    Listenelement* GetNachfolger(void);
    Listenelement* GetVorgaenger(void);
    T* GetElement(void);
    void Wert(double);
    double Wert(void);
};


template<class T> class Liste
{
  private:
      Listenelement<T>* erstesElement;
      Listenelement<T>*letztesElement;
      Listenelement<T>* aktuellesElement;
      int listenGroesse;
      void MengeInMitteTeilen(Listenelement<T> *Anfang, Listenelement<T> *Ende, double _min, double _max);
  public:
      Liste();
      Liste(const Liste&);
      ~Liste();
      Liste& operator=(const Liste&);
      Listenelement<T>* GetErstesListenelement(void);
      T* GetErstesElement(void);
      T* GetNaechstesElement(void);
      T* GetAktuellenInhalt(void);
      T* GetNachfolger(T*);
      T* GetVorgaenger(T*);
      void Hinzufuegen(T* XY, bool nachHinten = 1);
      void Entfernen(T*);
      void Entzufuegen(T*, bool nachHinten = 1);/*entfernen wenn enthalten und hinzufuegen wenn nicht enthalten (Toggle)*/
      bool ExklusivHinzufuegen(T*, bool nachHinten = 1);/*nur hinzufuegen, wenn noch nicht enthalten - true wenn noch nicht enthalten*/
      void Entfernen(Listenelement<T>*);
      void AktuellesEntfernen(void);
      void ListeLeeren(const char*);
      void ListeLoeschen(const char*);
      int GetListenGroesse(void);
      void Tauschen(Listenelement<T>*, Listenelement<T>*);
      void Hintenanstellen(Listenelement<T>*, Listenelement<T>*);
      void Voranstellen(Listenelement<T>*, Listenelement<T>*);
      void AnsEndeStellen(T*);
      void Wert(T*, double);
      double Wert(T*);
      void ListeNachWertSortieren();

      /*Logbuch*/
      void logSchreiben(const char*);
      void logSchreiben(const double msg, int i=0);
      void logSchreiben(const void *msg);
      /*ENDE Logbuch*/
      void ListenInfo(const char* msg);
};

/*Listenelement*/
template<class T> Listenelement<T>::Listenelement()
{
  Element = 0;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> Listenelement<T>::Listenelement(T* XY)
{
  Element = XY;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> Listenelement<T>::Listenelement(const Listenelement& Vorlage)
{
    Element = Vorlage.Element;
    this->SetNachfolger(Vorlage.GetNachfolger());
    this->SetVorgaenger(Vorlage.GetVorgaenger());
}

template<class T> Listenelement<T>::~Listenelement()
{
    if(Vorgaenger)
    {
        Vorgaenger->SetNachfolger(Nachfolger);
    }
    if(Nachfolger)
    {
        Nachfolger->SetVorgaenger(Vorgaenger);
    }
}

template<class T> const Listenelement<T>& Listenelement<T>::operator=(const Listenelement& Vorlage) const
{
    if(this==&Vorlage)return *this;
    Element = Vorlage.Element;
    this->SetNachfolger(Vorlage.GetNachfolger());
    this->SetVorgaenger(Vorlage.GetVorgaenger());
    return *this;
}

template<class T> T* Listenelement<T>::GetElement(void)
{
  return(Element);
}

template<class T> void Listenelement<T>::SetNachfolger(Listenelement* NeuerNachfolger)
{
  Nachfolger = NeuerNachfolger;
  return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetNachfolger(void)
{
  return(Nachfolger);
}

template<class T> void Listenelement<T>::SetVorgaenger(Listenelement* NeuerVorgaenger)
{
    Vorgaenger = NeuerVorgaenger;
    return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetVorgaenger(void)
{
  return(Vorgaenger);
}

template<class T> void Listenelement<T>::Wert(double a)
{
    m_Wert = a;
    return;
}

template<class T> double Listenelement<T>::Wert(void)
{
    return m_Wert;
}

/*Ende Listenelement*/

/*Liste*/
template<class T> Liste<T>::Liste()
{
    erstesElement = 0;
    letztesElement = 0;
    aktuellesElement = 0;
    listenGroesse = 0;
}

template<class T> Liste<T>::Liste(const Liste& Vorlage)
{
    //ListeLoeschen("CopyCTOR");
    ListeLeeren("CopyCTOR");
    erstesElement = 0;
    letztesElement = 0;
    aktuellesElement = 0;
    listenGroesse = 0;
    for(Listenelement<T>* SuchElement=Vorlage.erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        Hinzufuegen(SuchElement->GetElement());
    }
}

template<class T> Liste<T>::~Liste()
{
    ListeLeeren("DTOR");
    //cout<<"Liste loeschen\n";
}

template<class T> Liste<T>& Liste<T>::operator=(const Liste& Vorlage)
{
    if(this == &Vorlage)return *this;
    //ListeLoeschen("operator=");
    ListeLeeren("operator=");
    erstesElement = 0;
    letztesElement = 0;
    aktuellesElement = 0;
    listenGroesse = 0;
    for(Listenelement<T>* SuchElement=Vorlage.erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        Hinzufuegen(SuchElement->GetElement());
    }
    return *this;
}

template<class T> T* Liste<T>::GetErstesElement(void)
{
    aktuellesElement = erstesElement;
    return(GetAktuellenInhalt());
}

template<class T> Listenelement<T>* Liste<T>::GetErstesListenelement(void)
{
    return(erstesElement);
}

template<class T> T* Liste<T>::GetNaechstesElement(void)
{
    if(aktuellesElement!=NULL)
    {
        aktuellesElement = aktuellesElement->GetNachfolger();
        return(GetAktuellenInhalt());
    }else{
		return(GetErstesElement());
	}
    return NULL;
}

template<class T> T* Liste<T>::GetAktuellenInhalt(void)
{
    if(aktuellesElement!=NULL){
        return(aktuellesElement->GetElement());
    }else{
        return NULL;
    }
}

template<class T> void Liste<T>::Hinzufuegen(T* XY, bool nachHinten)
{
    if(XY == NULL)return;
    Listenelement<T>* neuesElement = new Listenelement<T>(XY);

    if(nachHinten)
    {
        if(letztesElement)letztesElement->SetNachfolger(neuesElement);
        neuesElement->SetVorgaenger(letztesElement);
        letztesElement = neuesElement;
        if(!erstesElement)erstesElement = neuesElement;
    }else{
        if(erstesElement)erstesElement->SetVorgaenger(neuesElement);
        neuesElement->SetNachfolger(erstesElement);
        erstesElement = neuesElement;
        if(!letztesElement)letztesElement = neuesElement;
    }
    listenGroesse++;
    return;
}

template<class T> void Liste<T>::Entfernen(T* XY)
{
    if(!(erstesElement))return;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            if(SuchElement == erstesElement)
            {
                erstesElement = SuchElement->GetNachfolger();
            }
            if(SuchElement == letztesElement)
            {
                letztesElement = SuchElement->GetVorgaenger();
            }
            if(SuchElement == aktuellesElement)
            {
                aktuellesElement = SuchElement->GetVorgaenger();
            }
            delete SuchElement;
            SuchElement = NULL;
            listenGroesse--;
            return;
        }
    }
    return;
}

template<class T> void Liste<T>::Entzufuegen(T* XY, bool nachHinten)
{
    if(!(erstesElement))
    {
        Hinzufuegen(XY);
        return;
    }

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            if(SuchElement == erstesElement)
            {
                erstesElement = SuchElement->GetNachfolger();
            }
            if(SuchElement == letztesElement)
            {
                letztesElement = SuchElement->GetVorgaenger();
            }
            if(SuchElement == aktuellesElement)
            {
                aktuellesElement = SuchElement->GetVorgaenger();
            }
            delete SuchElement;
            SuchElement = NULL;
            listenGroesse--;
            return;
        }
    }
    Hinzufuegen(XY, nachHinten);
    return;
}

template<class T> bool Liste<T>::ExklusivHinzufuegen(T* XY, bool nachHinten)
{
    if(!(erstesElement))
    {
        Hinzufuegen(XY);
        return true;
    }

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            return false;
        }
    }
    Hinzufuegen(XY, nachHinten);
    return true;
}

template<class T> void Liste<T>::Entfernen(Listenelement<T> *XY)
{
    if(!(erstesElement))return;
    if(XY == erstesElement)
    {
        erstesElement = XY->GetNachfolger();
    }
    if(XY == letztesElement)
    {
        letztesElement = XY->GetVorgaenger();
    }
    if(XY == aktuellesElement)
    {
        aktuellesElement = XY->GetVorgaenger();
    }
    delete XY;
    XY = NULL;
    listenGroesse--;
    return;
}

template<class T> void Liste<T>::Hintenanstellen(Listenelement<T>* Wanderer, Listenelement<T>* Anker)
{
    if(Anker == Wanderer)return;
    if(Anker!=NULL)
    {
        if(Anker->GetNachfolger() == Wanderer)return;
        if(Wanderer == erstesElement)erstesElement = Wanderer->GetNachfolger();
        if(Wanderer == letztesElement)letztesElement = Wanderer->GetVorgaenger();
        if(Anker == letztesElement)letztesElement = Wanderer;

        Listenelement<T> *vw, *nw, *na;
        vw = Wanderer->GetVorgaenger();
        nw = Wanderer->GetNachfolger();
        na = Anker->GetNachfolger();

        if(nw == Anker)
        {
            Anker->SetVorgaenger(vw);
            if(vw != NULL)vw->SetNachfolger(Anker);

            Wanderer->SetNachfolger(na);
            if(na != NULL)na->SetVorgaenger(Wanderer);

            Anker->SetNachfolger(Wanderer);
            Wanderer->SetVorgaenger(Anker);
        }else{

            if(vw != NULL)vw->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(vw);

            Wanderer->SetVorgaenger(Anker);
            Wanderer->SetNachfolger(na);
            Anker->SetNachfolger(Wanderer);
            if(na != NULL)na->SetVorgaenger(Wanderer);
        }
    }
    return;
}

template<class T> void Liste<T>::Voranstellen(Listenelement<T>* Wanderer, Listenelement<T>* Anker)
{
    if(Anker == Wanderer)return;
    if(Anker!=NULL)
    {
        if(Anker->GetVorgaenger()==Wanderer)return;
        if(Anker == erstesElement)erstesElement = Wanderer;
        if(Wanderer == letztesElement)letztesElement = Wanderer->GetVorgaenger();

        Listenelement<T> *vw, *nw, *va;
        vw = Wanderer->GetVorgaenger();
        nw = Wanderer->GetNachfolger();
        va = Anker->GetVorgaenger();

        if(vw==Anker)
        {
            Anker->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(Anker);

            Wanderer->SetVorgaenger(va);
            if(va != NULL)va->SetNachfolger(Wanderer);

            Anker->SetVorgaenger(Wanderer);
            Wanderer->SetNachfolger(Anker);
        }else{
            if(vw != NULL)vw->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(vw);

            Wanderer->SetVorgaenger(va);
            Wanderer->SetNachfolger(Anker);

            Anker->SetVorgaenger(Wanderer);
            if(va != NULL)va->SetNachfolger(Wanderer);
        }
    }
    return;
}

template<class T> void Liste<T>::AnsEndeStellen(T* XY)
{
    if(!(erstesElement))return;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            Hintenanstellen(SuchElement, letztesElement);
            break;
        }
    }
    return;
}

template<class T> void Liste<T>::Wert(T* XY, double t_wert)
{
    if(!(erstesElement))return;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            SuchElement->Wert(t_wert);
            return;
        }
    }
  return;
}

template<class T> double Liste<T>::Wert(T* XY)
{
    if(!(erstesElement))return 0.0;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            return SuchElement->Wert();
        }
    }
  return 0.0;
}

template<class T> void Liste<T>::AktuellesEntfernen(void)
{
    if(!(aktuellesElement))return;

    Listenelement<T>* SuchElement = aktuellesElement;
    if(SuchElement == erstesElement)
    {
        erstesElement = SuchElement->GetNachfolger();
    }
    if(SuchElement == letztesElement)
    {
        letztesElement = SuchElement->GetVorgaenger();
    }
    aktuellesElement = SuchElement->GetVorgaenger();

    delete SuchElement;
    SuchElement = 0;
    listenGroesse--;
    return;
}

template<class T> void Liste<T>::ListeLeeren(const char* von)
{
    while(erstesElement)
    {
        Entfernen(erstesElement);
    }
    return;
}

template<class T> void Liste<T>::ListeLoeschen(const char* von)
{
    while(erstesElement)
    {
        T *Inhalt = erstesElement->GetElement();
        delete Inhalt;
        Entfernen(erstesElement);
    }
    return;
}

template<class T> int Liste<T>::GetListenGroesse()
{
    return listenGroesse;
}

template<class T> void Liste<T>::Tauschen(Listenelement<T>* LE_Eins, Listenelement<T>* LE_Zwei)
{
    if(LE_Eins == LE_Zwei)return;
    if((LE_Eins == NULL) ||(LE_Zwei == NULL)) return;

    Listenelement<T> *V1, *N1, *V2, *N2;

    if(LE_Eins->GetNachfolger() == LE_Zwei)
    {
        V1 = LE_Eins->GetVorgaenger();
        N2 = LE_Zwei->GetNachfolger();

        LE_Eins->SetVorgaenger(LE_Zwei);
        LE_Zwei->SetNachfolger(LE_Eins);

        LE_Eins->SetNachfolger(N2);
        LE_Zwei->SetVorgaenger(V1);

        if(V1)V1->SetNachfolger(LE_Zwei);
        if(N2)N2->SetVorgaenger(LE_Eins);
    }
    else if(LE_Zwei->GetNachfolger() == LE_Eins)
    {
        V2 = LE_Zwei->GetVorgaenger();
        N1 = LE_Eins->GetNachfolger();

        LE_Zwei->SetVorgaenger(LE_Eins);
        LE_Eins->SetNachfolger(LE_Zwei);

        LE_Zwei->SetNachfolger(N1);
        LE_Eins->SetVorgaenger(V2);

        if(V2)V2->SetNachfolger(LE_Eins);
        if(N1)N1->SetVorgaenger(LE_Zwei);
    }else
    {
        V1 = LE_Eins->GetVorgaenger();
        N1 = LE_Eins->GetNachfolger();
        V2 = LE_Zwei->GetVorgaenger();
        N2 = LE_Zwei->GetNachfolger();

        LE_Zwei->SetVorgaenger(V1);
        LE_Zwei->SetNachfolger(N1);
        LE_Eins->SetVorgaenger(V2);
        LE_Eins->SetNachfolger(N2);

        if(V1)V1->SetNachfolger(LE_Zwei);
        if(N1)N1->SetVorgaenger(LE_Zwei);
        if(V2)V2->SetNachfolger(LE_Eins);
        if(N2)N2->SetVorgaenger(LE_Eins);
    }
    if(erstesElement == LE_Eins)
    {
        erstesElement = LE_Zwei;
    }else
    if(erstesElement == LE_Zwei)
    {
        erstesElement = LE_Eins;
    }

    if(letztesElement == LE_Eins)
    {
        letztesElement = LE_Zwei;
    }else
    if(letztesElement == LE_Zwei)
    {
        letztesElement = LE_Eins;
    }
    return;
}


template<class T> T* Liste<T>::GetNachfolger(T* ausgangsElement)
{
    T* nachfolger = NULL;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == ausgangsElement)
        {
            if(SuchElement->GetNachfolger())
            nachfolger = SuchElement->GetNachfolger()->GetElement();
        }
    }
    return nachfolger;
}

template<class T> T* Liste<T>::GetVorgaenger(T* ausgangsElement)
{
    T* vorgaenger = NULL;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == ausgangsElement)
        {
            if(SuchElement->GetVorgaenger())
            vorgaenger = SuchElement->GetVorgaenger()->GetElement();
        }
    }
    return vorgaenger;
}

template<class T> void Liste<T>::MengeInMitteTeilen(Listenelement<T> *Anfang, Listenelement<T> *Ende, double _min, double _max)
{
    Listenelement<T> *LE_Anfang = Anfang;
    Listenelement<T> *LE_Ende = Ende;
    double minWert = _min;
    double maxWert = _max;
    double groessterMinWert = _min;
    double kleinsterMaxWert = _max;
    double mittelWert, tempWert;

    /*Prüfen, ob der Teilabschnittsortiert ist*/
    if(LE_Anfang == LE_Ende)return;
    if((LE_Anfang == NULL)||(LE_Ende == NULL))return;
    if(LE_Ende == LE_Anfang->GetNachfolger())
    {
        if(LE_Ende->Wert()<LE_Anfang->Wert())Voranstellen(LE_Ende, LE_Anfang);
        return;
    }
    /*Prüfen auf Rundungsprobleme oder Grenzgleichheit*/
    mittelWert = (minWert+maxWert)/2.0;
    if((mittelWert == minWert) || (mittelWert == maxWert)||(minWert == maxWert))
    {
        /*Grenzen Stossen aneinander*/
        return;
    }
    /*Ende der Prüfungen*/

    /*Abschnitt aufteilen - kleine Wert nach vorne, große nach hinten*/
    Listenelement<T> *laeufer, *tempLE, *LE_neuesEnde, *LE_neuerAnfang;

    laeufer = LE_Anfang;
    LE_neuesEnde = LE_Ende;

    while(laeufer != LE_Ende)
    {
        tempLE = laeufer;
        laeufer = laeufer->GetNachfolger();
        tempWert = tempLE->Wert();
        if(tempWert < mittelWert)
        {
            if(groessterMinWert < tempWert)groessterMinWert = tempWert;
            if(tempLE != LE_Anfang)
            {
                Voranstellen(tempLE, LE_Anfang);
                LE_Anfang = tempLE;
            }
        }else{
            if(kleinsterMaxWert > tempWert)kleinsterMaxWert = tempWert;
            if(tempLE == LE_Anfang)
            {
                LE_Anfang = LE_Anfang->GetNachfolger();
            }
            Hintenanstellen(tempLE, LE_neuesEnde);
            LE_neuesEnde = tempLE;
        }
    }

    tempWert = LE_Ende->Wert();
    if(tempWert < mittelWert)
    {
        if(groessterMinWert < tempWert)groessterMinWert = tempWert;
        if(LE_Ende == LE_neuesEnde)
        {
            LE_neuesEnde = LE_neuerAnfang = NULL;
            MengeInMitteTeilen(LE_Anfang, LE_Ende, minWert, groessterMinWert);
            return;
        }else{
            LE_neuerAnfang = LE_Ende->GetNachfolger();
        }
    }else{
        if(kleinsterMaxWert > tempWert)kleinsterMaxWert = tempWert;
        LE_neuerAnfang = LE_Ende;
        if(LE_Ende != LE_Anfang)
        {
            LE_Ende = LE_Ende->GetVorgaenger();
        }else{
            LE_Ende = LE_Anfang = NULL;
            MengeInMitteTeilen(LE_neuerAnfang, LE_neuesEnde, kleinsterMaxWert, maxWert);
            return;
        }
    }

    /*Teilabschnitte wieder unterteilen*/
    MengeInMitteTeilen(LE_Anfang, LE_Ende, minWert, groessterMinWert);
    MengeInMitteTeilen(LE_neuerAnfang, LE_neuesEnde, kleinsterMaxWert, maxWert);
    return;
}

template<class T> void Liste<T>::ListeNachWertSortieren()
{
    Listenelement<T> *laeufer, *Ende, *Anfang;
    double minWert, maxWert, tempWert;

    laeufer = erstesElement;
    if(laeufer != NULL)
    {
        Ende = laeufer;
        Anfang = laeufer;
        minWert = maxWert = laeufer->Wert();
        laeufer = laeufer->GetNachfolger();
    }else{
        return;
    }

    while(laeufer != NULL)
    {
        Ende = laeufer;
        tempWert = laeufer->Wert();
        if(minWert > tempWert) minWert = tempWert;
        if(maxWert < tempWert) maxWert = tempWert;
        laeufer = laeufer->GetNachfolger();
    };
    MengeInMitteTeilen(Anfang, Ende, minWert, maxWert);
    return;
}

template<class T> void Liste<T>::ListenInfo(const char* msg)
{
    logSchreiben("\n");logSchreiben(msg);logSchreiben("\n");
    for(Listenelement<T>* t_LE = erstesElement; t_LE; t_LE = t_LE->GetNachfolger())
    {
        logSchreiben(t_LE);logSchreiben("(");logSchreiben(t_LE->GetElement());logSchreiben("); ");
    }
    logSchreiben("\n");
    logSchreiben("erstesElement: ");logSchreiben(erstesElement);logSchreiben("\n");
    logSchreiben("letztesElement: ");logSchreiben(letztesElement);logSchreiben("\n");
    logSchreiben("\n");
}


/*Logbuch*/
template<class T> void Liste<T>::logSchreiben(const char*msg)
{
    ofstream Logbuch;
    const char *pfad = "log/Listen.log";
    Logbuch.open(pfad, ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

template<class T> void Liste<T>::logSchreiben(const double msg, int i)
{
    ofstream Logbuch;
    const char *pfad = "log/Listen.log";
    Logbuch.open(pfad, ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch.setf( ios::fixed, ios::floatfield );
        Logbuch.precision(i);
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

template<class T> void Liste<T>::logSchreiben(const void *msg)
{
    ofstream Logbuch;
    const char *pfad = "log/Listen.log";
    Logbuch.open(pfad, ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}
/*ENDE Logbuch*/

#endif
