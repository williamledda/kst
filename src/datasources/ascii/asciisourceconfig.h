/***************************************************************************
 *                                                                         *
 *   Copyright : (C) 2010 The University of Toronto                        *
 *   email     : netterfield@astro.utoronto.ca                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KST_ASCII_SOURCE_CONFIG_H
#define KST_ASCII_SOURCE_CONFIG_H

#include "namedparameter.h"

#include <QDomElement>
#include <QDateTime>


class AsciiSourceConfig {

    // TODO translate keys?
    static const char Key_fileNamePattern[];
    static const char Tag_fileNamePattern[];
    static const char Key_delimiters[];
    static const char Tag_delimiters[];
    static const char Key_indexVector[];
    static const char Tag_indexVector[];
    static const char Key_indexInterpretation[];
    static const char Tag_indexInterpretation[];
    static const char Key_timeAsciiFormatString[];
    static const char Tag_timeAsciiFormatString[];
    static const char Key_columnType[];
    static const char Tag_columnType[];
    static const char Key_columnDelimiter[];
    static const char Tag_columnDelimiter[];
    static const char Key_columnWidth[];
    static const char Tag_columnWidth[];
    static const char Key_columnWidthIsConst[];
    static const char Tag_columnWidthIsConst[];
    static const char Key_dataLine[];
    static const char Tag_dataLine[];
    static const char Key_readFields[];
    static const char Tag_readFields[];
    static const char Key_readUnits[];
    static const char Tag_readUnits[];
    static const char Key_useDot[];
    static const char Tag_useDot[];
    static const char Key_fieldsLine[];
    static const char Tag_fieldsLine[];
    static const char Key_unitsLine[];
    static const char Tag_unitsLine[];
    static const char Key_limitFileBuffer[];
    static const char Tag_limitFileBuffer[];
    static const char Key_limitFileBufferSize[];
    static const char Tag_limitFileBufferSize[];
    static const char Key_useThreads[];
    static const char Tag_useThreads[];
    static const char Key_fileRolling[];
    static const char Tag_fileRolling[];
    static const char Key_dataRate[];
    static const char Tag_dataRate[];
    static const char Key_offsetDateTime[];
    static const char Tag_offsetDateTime[];
    static const char Key_offsetFileDate[];
    static const char Tag_offsetFileDate[];
    static const char Key_offsetRelative[];
    static const char Tag_offsetRelative[];
    static const char Key_dateTimeOffset[];
    static const char Tag_dateTimeOffset[];
    static const char Key_relativeOffset[];
    static const char Tag_relativeOffset[];
    static const char Key_nanValue[];
    static const char Tag_nanValue[];
    static const char Key_updateType[];
    static const char Tag_updateType[];

  public:
    AsciiSourceConfig();

    bool operator==(const AsciiSourceConfig&) const;
    bool operator!=(const AsciiSourceConfig& rhs) const;
    bool isUpdateNecessary(const AsciiSourceConfig& rhs) const;

    void saveDefault(QSettings& cfg) const;
    void saveGroup(QSettings& cfg, const QString& fileName) const;
    const AsciiSourceConfig& readGroup(QSettings& cfg, const QString& fileName = QString());

    void save(QXmlStreamWriter& s);
    void parseProperties(QXmlStreamAttributes &properties);

    void load(const QDomElement& e);

    enum Interpretation { Unknown = 0, NoInterpretation, CTime, Seconds, FormattedTime, FixedRate, IntEnd = 0xffff };
    enum ColumnType { Whitespace = 0, Fixed, Custom, ColEnd = 0xffff };

    NamedParameter<QString, Key_delimiters, Tag_delimiters> _delimiters;
    NamedParameter<QString, Key_indexVector, Tag_indexVector> _indexVector;
    NamedParameter<int, Key_indexInterpretation, Tag_indexInterpretation> _indexInterpretation;
    NamedParameter<QString, Key_timeAsciiFormatString, Tag_timeAsciiFormatString> _timeAsciiFormatString;
    NamedParameter<QString, Key_fileNamePattern, Tag_fileNamePattern> _fileNamePattern;
    NamedParameter<int, Key_columnType, Tag_columnType> _columnType;
    NamedParameter<QString, Key_columnDelimiter, Tag_columnDelimiter> _columnDelimiter;
    NamedParameter<int, Key_columnWidth, Tag_columnWidth> _columnWidth;
    NamedParameter<bool, Key_columnWidthIsConst, Tag_columnWidthIsConst> _columnWidthIsConst;
    NamedParameter<int, Key_dataLine, Tag_dataLine> _dataLine;
    NamedParameter<bool, Key_readFields, Tag_readFields> _readFields;
    NamedParameter<bool, Key_readUnits, Tag_readUnits> _readUnits;
    NamedParameter<int, Key_fieldsLine, Tag_fieldsLine> _fieldsLine;
    NamedParameter<int, Key_unitsLine, Tag_unitsLine> _unitsLine;
    NamedParameter<bool, Key_useDot, Tag_useDot> _useDot;
    NamedParameter<bool, Key_limitFileBuffer, Tag_limitFileBuffer> _limitFileBuffer;
    NamedParameter<qint64, Key_limitFileBufferSize, Tag_limitFileBufferSize> _limitFileBufferSize;
    NamedParameter<int, Key_useThreads, Tag_useThreads> _useThreads;
    NamedParameter<int, Key_fileRolling, Tag_fileRolling> _fileRolling;
    NamedParameter<double, Key_dataRate, Tag_dataRate> _dataRate;
    NamedParameter<bool, Key_offsetDateTime, Tag_offsetDateTime> _offsetDateTime;
    NamedParameter<bool, Key_offsetFileDate, Tag_offsetFileDate> _offsetFileDate;
    NamedParameter<bool, Key_offsetRelative, Tag_offsetRelative> _offsetRelative;
    NamedParameter<QDateTime, Key_dateTimeOffset, Tag_dateTimeOffset> _dateTimeOffset;
    NamedParameter<double, Key_relativeOffset, Tag_relativeOffset> _relativeOffset;
    NamedParameter<int, Key_nanValue, Tag_nanValue> _nanValue;
    NamedParameter<int, Key_updateType, Tag_updateType> _updateType;

  private:
    void save(QSettings& cfg) const;
    void read(QSettings& cfg);
};

Q_DECLARE_METATYPE(AsciiSourceConfig::Interpretation)
Q_DECLARE_METATYPE(AsciiSourceConfig::ColumnType)

#endif
