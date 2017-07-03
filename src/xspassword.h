#ifndef XSPASSWORD_H
#define XSPASSWORD_H

#include "xslib_global.h"
#include <QCryptographicHash>
#include <QFile>

#define MAX_UNICODE 65536
#define MAX_ASCII 255

class XSLIBSHARED_EXPORT xsPassword
{
public:
    xsPassword();
    xsPassword(const QString &passwd, bool copyClear = false, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    /* Check if @hit password is equivalent of #strPassword
     * W: All attemps will be count and if @iHit is higher than @iMaxHit return is always #FAIL
     * @hit Password to check
     */
    int Check(const QString &hit);
    /* Check if @hit password is equivalent of #strPassword
     * W: All attemps will be count and if @iHit is higher than @iMaxHit return is always #FAIL
     * @hit Password to check
     */
    int Check(QFile &hit);
    /* Check if @hit password is equivalent of #strPassword
     * W: All attemps will be count and if @iHit is higher than @iMaxHit return is always #FAIL
     * @hit Password to check
     */
    int Check(const xsPassword &hit);
    /* Write hashed password #strPassword with #iType as alghoritm on @pathfile
     * @pathfile Path of file where password will be write
     */
    int Save(const QString &pathfile) const;
    /* Read hashed password @passwd with @type as alghoritm on @file
     * @file File where password will be read
     * @type New #iType value
     * @maxhit New #iMaxHit value
     */
    int Load(QFile &file, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    /* Write hashed password @passwd with @type as alghoritm on @pathfile
     * @pathfile Path of file where password will be write
     * @passwd Source of the password to storage
     * @type Algorithm to use when @passwd is hashed
     */
    static int Save(const QString &pathfile, QString &passwd, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512);
    /* Read and Return the content of @pathfile
     * @pathfile Path of file to read
     */
    static QString Load(const QString &pathfile);
    /* Change password storaged in #strPassword with hashed @passwd using #iType algorithm
     * @passwd Source of new password
     * @copyClear Will copy @passwd without hash in #strClear?
     */
    int setPassword(const QString &passwd, bool copyClear = false, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    /* Change algorithm to hash storaged in #iType
     * @type New algorithm
     */
    void setAlgorithm(QCryptographicHash::Algorithm type);
    /* Return hashed password storaged in #strPassword
     */
    QString getPassword() const;
    /* Return unhashed password storaged in #strClear
     */
    QString getClearPassword() const;
    /* Return current number of attempts using #Check(*)
     */
    int getHit() const;
    /* Return maximum limit of attempts using #Check(*)
     */
    int getMaxHit()const;
    /* Set new maximum limit of attempts using #Check(*)
     * @n New #iMaxHit
     */
    int setMaxHit(int n);
    /* Generate an hashed string of @key with @type algorithm
     * @key Source string to hash
     * @type Algorithm to use when hashing
     */
    static QString HashKey(const QString &key, QCryptographicHash::Algorithm type);
    /* Generate a new random password as output QString
     * @length Length of the output
     * @symbols Output will contain symbols characters?
     * @spaces Output will contain space characters?
     * @unicode Is output string in UTF-16?
     * @numbers Output will contain digit characters?
     * @lowers Output will contain lower characters?
     * @uppers Output will contain uppers characters?
     */
    static QString generate(int length, bool symbols = false, bool spaces = false, bool unicode = false, bool numbers = true, bool lowers = true, bool uppers = true);
private:
    QCryptographicHash::Algorithm iType;
    QString strPassword;
    QString strClear;
    int iMaxHit;
    int iHit;
};

#endif // XSPASSWORD_H
