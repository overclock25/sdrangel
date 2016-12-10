///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 F4EXB                                                      //
// written by Edouard Griffiths                                                  //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <QChar>
#include <QDebug>
#include "cwkeyer.h"

/**
 * 0:  dot
 * 1:  dash
 * -1: end of sequence
 */
const char CWKeyer::m_asciiToMorse[128][7] = {
        {-1,0,0,0,0,0,0}, // 0
        {-1,0,0,0,0,0,0}, // 1
        {-1,0,0,0,0,0,0}, // 2
        {-1,0,0,0,0,0,0}, // 3
        {-1,0,0,0,0,0,0}, // 4
        {-1,0,0,0,0,0,0}, // 5
        {-1,0,0,0,0,0,0}, // 6
        {-1,0,0,0,0,0,0}, // 7
        {-1,0,0,0,0,0,0}, // 8
        {-1,0,0,0,0,0,0}, // 9
        {-1,0,0,0,0,0,0}, // 10
        {-1,0,0,0,0,0,0}, // 11
        {-1,0,0,0,0,0,0}, // 12
        {-1,0,0,0,0,0,0}, // 13
        {-1,0,0,0,0,0,0}, // 14
        {-1,0,0,0,0,0,0}, // 15
        {-1,0,0,0,0,0,0}, // 16
        {-1,0,0,0,0,0,0}, // 17
        {-1,0,0,0,0,0,0}, // 18
        {-1,0,0,0,0,0,0}, // 19
        {-1,0,0,0,0,0,0}, // 20
        {-1,0,0,0,0,0,0}, // 21
        {-1,0,0,0,0,0,0}, // 22
        {-1,0,0,0,0,0,0}, // 23
        {-1,0,0,0,0,0,0}, // 24
        {-1,0,0,0,0,0,0}, // 25
        {-1,0,0,0,0,0,0}, // 26
        {-1,0,0,0,0,0,0}, // 27
        {-1,0,0,0,0,0,0}, // 28
        {-1,0,0,0,0,0,0}, // 29
        {-1,0,0,0,0,0,0}, // 30
        {-1,0,0,0,0,0,0}, // 31
        {-1,0,0,0,0,0,0}, // 32 space is treated as word separator
        {1,0,1,0,1,1,-1}, // 33 !
        {0,1,0,0,1,0,-1}, // 34 "
        {-1,0,0,0,0,0,0}, // 35
        {-1,0,0,0,0,0,0}, // 36
        {-1,0,0,0,0,0,0}, // 37
        {-1,0,0,0,0,0,0}, // 38
        {0,1,1,1,1,0,-1}, // 39 '
        {1,0,1,1,0,1,-1}, // 40 (
        {1,0,1,1,0,1,-1}, // 41 )
        {-1,0,0,0,0,0,0}, // 42
        {0,1,0,1,0,-1,0}, // 43 +
        {1,1,0,0,1,1,-1}, // 44 ,
        {1,0,0,0,0,1,-1}, // 45 -
        {0,1,0,1,0,1,-1}, // 46 .
        {1,0,0,1,0,-1,0}, // 47 /
        {1,1,1,1,1,-1,0}, // 48 0
        {0,1,1,1,1,-1,0}, // 49 1
        {0,0,1,1,1,-1,0}, // 50 2
        {0,0,0,1,1,-1,0}, // 51 3
        {0,0,0,0,1,-1,0}, // 52 4
        {0,0,0,0,0,-1,0}, // 53 5
        {1,0,0,0,0,-1,0}, // 54 6
        {1,1,0,0,0,-1,0}, // 55 7
        {1,1,1,0,0,-1,0}, // 56 8
        {1,1,1,1,0,-1,0}, // 57 9
        {1,1,1,0,0,0,-1}, // 58 :
        {1,0,1,0,1,0,-1}, // 59 ;
        {-1,0,0,0,0,0,0}, // 60 <
        {1,0,0,0,1,-1,0}, // 61 =
        {-1,0,0,0,0,0,0}, // 62 >
        {0,0,1,1,0,0,-1}, // 63 ?
        {0,1,1,0,1,0,-1}, // 64 @
        {0,1,-1,0,0,0,0}, // 65 A
        {1,0,0,0,-1,0,0}, // 66 B
        {1,0,1,0,-1,0,0}, // 67 C
        {1,0,0,-1,0,0,0}, // 68 D
        {0,-1,0,0,0,0,0}, // 69 E
        {0,0,1,0,-1,0,0}, // 70 F
        {1,1,0,-1,0,0,0}, // 71 G
        {0,0,0,0,-1,0,0}, // 72 H
        {0,0,-1,0,0,0,0}, // 73 I
        {0,1,1,1,-1,0,0}, // 74 J
        {1,0,1,-1,0,0,0}, // 75 K
        {0,1,0,0,-1,0,0}, // 76 L
        {1,1,-1,0,0,0,0}, // 77 M
        {1,0,-1,0,0,0,0}, // 78 N
        {1,1,1,-1,0,0,0}, // 79 O
        {0,1,1,0,-1,0,0}, // 80 P
        {1,1,0,1,-1,0,0}, // 81 Q
        {0,1,0,-1,0,0,0}, // 82 R
        {0,0,0,-1,0,0,0}, // 83 S
        {1,-1,0,0,0,0,0}, // 84 T
        {0,0,1,-1,0,0,0}, // 85 U
        {0,0,0,1,-1,0,0}, // 86 V
        {0,1,1,-1,0,0,0}, // 87 W
        {1,0,0,1,-1,0,0}, // 88 X
        {1,0,1,1,-1,0,0}, // 89 Y
        {1,1,0,0,-1,0,0}, // 90 Z
        {-1,0,0,0,0,0,0}, // 91 [
        {-1,0,0,0,0,0,0}, // 92 back /
        {-1,0,0,0,0,0,0}, // 93 ]
        {-1,0,0,0,0,0,0}, // 94 ^
        {-1,0,0,0,0,0,0}, // 95 _
        {-1,0,0,0,0,0,0}, // 96 `
        {0,1,-1,0,0,0,0}, // 97 A lowercase same as uppercase
        {1,0,0,0,-1,0,0}, // 98 B
        {1,0,1,0,-1,0,0}, // 99 C
        {1,0,0,-1,0,0,0}, // 100 D
        {0,-1,0,0,0,0,0}, // 101 E
        {0,0,1,0,-1,0,0}, // 102 F
        {1,1,0,-1,0,0,0}, // 103 G
        {0,0,0,0,-1,0,0}, // 104 H
        {0,0,-1,0,0,0,0}, // 105 I
        {0,1,1,1,-1,0,0}, // 106 J
        {1,0,1,-1,0,0,0}, // 107 K
        {0,1,0,0,-1,0,0}, // 108 L
        {1,1,-1,0,0,0,0}, // 109 M
        {1,0,-1,0,0,0,0}, // 110 N
        {1,1,1,-1,0,0,0}, // 111 O
        {0,1,1,0,-1,0,0}, // 112 P
        {1,1,0,1,-1,0,0}, // 113 Q
        {0,1,0,-1,0,0,0}, // 114 R
        {0,0,0,-1,0,0,0}, // 115 S
        {1,-1,0,0,0,0,0}, // 116 T
        {0,0,1,-1,0,0,0}, // 117 U
        {0,0,0,1,-1,0,0}, // 118 V
        {0,1,1,-1,0,0,0}, // 119 W
        {1,0,0,1,-1,0,0}, // 120 X
        {1,0,1,1,-1,0,0}, // 121 Y
        {1,1,0,0,-1,0,0}, // 122 Z
        {-1,0,0,0,0,0,0}, // 123 {
        {-1,0,0,0,0,0,0}, // 124 |
        {-1,0,0,0,0,0,0}, // 125 }
        {-1,0,0,0,0,0,0}, // 126 ~
        {-1,0,0,0,0,0,0}, // 127 DEL
};

CWKeyer::CWKeyer() :
    m_sampleRate(48000),
    m_textPointer(0),
	m_elementPointer(0),
    m_samplePointer(0),
    m_elementSpace(false),
    m_characterSpace(false),
    m_key(false),
    m_dot(false),
    m_dash(false),
    m_elementOn(false),
	m_asciiChar('\0'),
    m_mode(CWKey),
    m_keyIambicState(KeySilent),
	m_textState(TextStart)
{
    setWPM(13);
}

CWKeyer::~CWKeyer()
{
}

void CWKeyer::setWPM(int wpm)
{
    if ((wpm > 0) && (wpm < 21))
    {
        m_wpm = wpm;
        m_dotLength = (int) (0.24f * m_sampleRate * (wpm / 5.0f));
    }
}

void CWKeyer::setDot(bool dotOn)
{
    if (dotOn)
    {
        m_dash = false;
        m_dot = true;
    }
    else
    {
        m_dot = false;
    }
}

void CWKeyer::setDash(bool dashOn)
{
    if (dashOn)
    {
        m_dot = false;
        m_dash = true;
    }
    else
    {
        m_dash = false;
    }
}

int CWKeyer::getSample()
{
    if (m_mode == CWKey)
    {
        return m_key ? 1 : 0;
    }
    else if (m_mode == CWIambic)
    {
    	nextStateIambic();
        return m_key ? 1 : 0;
    }
    else if (m_mode == CWText)
    {
    	nextStateText();
        return m_key ? 1 : 0;
    }
    else
    {
    	return 0;
    }
}

void CWKeyer::nextStateIambic()
{
    switch (m_keyIambicState)
    {
    case KeySilent:
        if (m_dot)
        {
            m_keyIambicState = KeyDot;
            m_samplePointer = 0;
        }
        else if (m_dash)
        {
            m_keyIambicState = KeyDash;
            m_samplePointer = 0;
        }

        m_key = false;
        break;
    case KeyDot:
        if (m_samplePointer < m_dotLength) // dot key
        {
            m_key = true;
            m_samplePointer++;
        }
        else if (m_samplePointer < 2*m_dotLength) // dot silence (+1 dot length)
        {
            m_key = false;
            m_samplePointer++;
        }
        else // end
        {
            if (m_dash)
            {
                m_samplePointer = 0;
                m_keyIambicState = KeyDash;
            }
            else if (!m_dot)
            {
                m_keyIambicState = KeySilent;
            }

            m_samplePointer = 0;
            m_key = false;
        }
        break;
    case KeyDash:
        if (m_samplePointer < 3*m_dotLength) // dash key
        {
            m_key = true;
            m_samplePointer++;
        }
        else if (m_samplePointer < 4*m_dotLength) // dash silence (+1 dot length)
        {
            m_key = false;
            m_samplePointer++;
        }
        else // end
        {
            if (m_dot)
            {
                m_samplePointer = 0;
                m_keyIambicState = KeyDot;
            }
            else if (!m_dash)
            {
                m_keyIambicState = KeySilent;
            }

            m_samplePointer = 0;
            m_key = false;
        }
        break;
    default:
        m_samplePointer = 0;
        m_key = false;
        break;
    }
}

void CWKeyer::nextStateText()
{
	// TODO...
	switch (m_textState)
	{
	case TextStart:
		m_samplePointer = 0;
		m_elementPointer = 0;
		m_textPointer = 0;
		m_textState = TextStartChar;
		break;
	case TextStartChar:
		m_samplePointer = 0;
		m_elementPointer = 0;
		if (m_textPointer < m_text.length())
		{
            m_asciiChar = (m_text.at(m_textPointer)).toLatin1();
//            qDebug() << "CWKeyer::nextStateText: TextStartChar: " << m_asciiChar;

            if (m_asciiChar < 0) { // non ASCII
                m_asciiChar = 0;
            }

            if (m_asciiChar == ' ')
            {
                m_textState = TextWordSpace;
            }
            else
            {
                m_textState = TextStartElement;
            }
            m_textPointer++;
		}
		else // end of text
		{
		    m_textState = TextEnd;
		}
		break;
	case TextStartElement:
		m_samplePointer = 0;
//        qDebug() << "CWKeyer::nextStateText: TextStartElement: " << (int) m_asciiToMorse[m_asciiChar][m_elementPointer];
		if (m_asciiToMorse[m_asciiChar][m_elementPointer] == -1) // end of morse character
		{
		    m_elementPointer = 0;
		    m_textState = TextCharSpace;
		}
		else
		{
            if (m_asciiToMorse[m_asciiChar][m_elementPointer] == 0) // dot
            {
                m_dot = true;
                m_dash = false;
            }
            else // dash
            {
                m_dot = false;
                m_dash = true;
            }
	        m_keyIambicState = KeySilent; // reset iambic state
	        nextStateIambic(); // init dash or dot
            m_dot = false; // release keys
            m_dash = false;
            m_textState = TextElement;
            m_elementPointer++;
		}
		break;
	case TextElement:
		nextStateIambic(); // dash or dot
		if (m_keyIambicState == KeySilent) // done
		{
			m_textState = TextStartElement; // next element
		}
		break;
	case TextCharSpace:
		if (m_samplePointer < 2*m_dotLength) // - 1 dot length space from element
		{
			m_samplePointer++;
			m_key = false;
		}
		else
		{
			m_textState = TextStartChar;
		}
		break;
	case TextWordSpace:
		if (m_samplePointer < 4*m_dotLength) // - 3 dot length space from character
		{
			m_samplePointer++;
			m_key = false;
		}
		else
		{
			m_textState = TextStartChar;
		}
		break;
	case TextEnd:
	default:
		m_key = false;
		break;
	}
}

bool CWKeyer::eom()
{
    return !(m_textPointer < m_text.length());
}
