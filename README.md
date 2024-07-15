# <a name="configure-group-claims-for-applications-with-azure-active-directory"></a>Configurer des revendications de groupe pour des applications avec Azure Active Directory

Azure Active Directory peut fournir une information d’appartenance de groupe d’utilisateurs 
dans des jetons utilisables au sein d’applications.  Deux modèles principaux sont pris en charge :

- Groupes identifiés par leur attribut d’identificateur d’objet Azure Active Directory
- Groupes identifiés par les attributs sAMAccountName ou GroupSID pour les groupes et utilisateurs synchronisés Active Directory (AD)

> **IMPORTANT**
> [!IMPORTANT]
> Il existe plusieurs mises en garde concernant cette fonctionnalité :
>

