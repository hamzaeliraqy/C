using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Lab4.Models
{
    public class Ads
    {
        public int AdsId
        {
            get;
            set;
        }

       
        [Required]
        [DisplayName("File Name")]
        public string FileName
        {
            get;
            set;
        }

        [Required]
        [DisplayName("Url")]
        public string Url
        {
            get;
            set;
        }
        public Community Community { get; set; }
        //public IEnumerable<CommunityMembership> CommunityMemberships { get; set; }
    }
}
