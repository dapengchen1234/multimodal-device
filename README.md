# <a name="configure-group-claims-for-applications-with-azure-active-directory"></a>Configurer des revendications de groupe pour des applications avec Azure Active Directory

Azure Active Directory peut fournir une information d’appartenance de groupe d’utilisateurs dans des jetons utilisables au sein d’applications.  Deux modèles principaux sont pris en charge :

- Groupes identifiés par leur attribut d’identificateur d’objet Azure Active Directory
- Groupes identifiés par les attributs sAMAccountName ou GroupSID pour les groupes et utilisateurs synchronisés Active Directory (AD)

> **IMPORTANT**
> [!IMPORTANT]
> Il existe plusieurs mises en garde concernant cette fonctionnalité :
>
>- La prise en charge de l’utilisation d’attributs sAMAccountName et d’identificateur de sécurité synchronisés localement est conçue pour permettre le déplacement d’applications existantes à partir d’AD FS et d’autres fournisseurs d’identité. Les groupes gérés dans Azure AD ne contiennent pas les attributs nécessaires pour émettre ces revendications.
>- Dans les grandes entreprises, le nombre de groupes dont un utilisateur est un membre peut dépasser la limite qu’Azure Active Directory ajoute à un jeton. 150 groupes pour un jeton SAML, et 200 pour un jeton JWT. Cela peut entraîner des résultats imprévisibles. Si vos utilisateurs ont un grand nombre d’appartenances à des groupes, nous vous recommandons d’utiliser l’option permettant de limiter les groupes émis dans les revendications aux groupes appropriés pour l’application.  
>- Pour un nouveau développement d’application, dans les cas où l’application peut être configurée pour celui-ci, et où une prise en charge de groupe imbriqué n’est pas obligatoire, il est recommandé que l’autorisation dans l’application soit basée sur les rôles d’application plutôt que sur des groupes.  Cela a pour effet de limiter la quantité d’informations qui doivent figurer dans le jeton, de sécuriser davantage et de séparer l’affectation d’utilisateurs de la configuration de l’application.
